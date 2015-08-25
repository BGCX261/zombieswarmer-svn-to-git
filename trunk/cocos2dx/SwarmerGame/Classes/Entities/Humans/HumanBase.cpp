#include "HumanBase.h"
#include "Entities/EntitySwarm.h"
#include "StateMachine/HumanStateMachine.h"
#include "Map/GameMap.h"
#include <assert.h>

static const int   kHumanHitPoints = 100;
static const float kHumanWalkSpeed = 0.01f;
static const float kHumanRunSpeed = 0.1f;
static const int   kHumanEndurance = 2;
static const int   kHumanRecovery = 5;

HumanBase::HumanBase(unsigned long id, EntityType type, MobileProperties mp)
   : MobileEntity(id, type, mp) 
   , m_HumanType (kHumanType_Fat)
   , m_bFleeing (false)
   , m_bBitten (false)
   , m_beginFleeRange (10000)
   , m_endFleeRange (20000)
{
   //m_sm = std::shared_ptr<HumanStateMachine>(new HumanStateMachine(this));
   setWeapon(WeaponFactory::GetWeapon(WeaponTypeHumanFists), MobileEntity::kWeapon_Default, -1);
}

HumanBase::~HumanBase ()
{
}


void HumanBase::step (float timeSlice)
{
   if (NULL != m_swarm)
   {
      stateMachine()()->execute(timeSlice);
   }
}

bool HumanBase::isDead () const
{
   return stateMachine().state() == HumanState::kDead;
}

bool HumanBase::isBitten() const
{
   return m_bBitten;
}

bool HumanBase::isZombie() const
{
   return stateMachine().state() == HumanState::kBitten;
}

bool HumanBase::hasTargetsInLoS () const
{
   if (NULL != m_swarm && m_swarm->gameMap())
   {
      std::vector< EntityBase* >::const_iterator it(m_swarm->targets().begin()), end(m_swarm->targets().end());
      for ( ; it != end; ++it)
      {
         if (m_swarm->gameMap()->checkLineOfSight(Position(x(), y()), Position((*it)->x(), (*it)->y()), std::numeric_limits<unsigned int>::max()))
            return true;
      }
   }
   return false;
}

bool HumanBase::targetInFleeRange () const
{
   int bestDist = std::numeric_limits<int>::max();
   if (NULL != m_swarm && m_swarm->gameMap())
   {
      std::vector< EntityBase* >::const_iterator targetIT (m_swarm->targets().begin()), end (m_swarm->targets().end());
      for ( ; targetIT != end; ++targetIT)
      {
         int distance = m_swarm->gameMap()->getEstimatedDistanceCost(m_x, m_y, (*targetIT)->x(), (*targetIT)->y());
         if (distance < getFleeRange () && distance < bestDist) 
            bestDist = distance;
      }
   }

   return (bestDist != std::numeric_limits<int>::max());
}

const EntityBase* HumanBase::getBestTarget () const
{
   EntityBase* bestTarget = NULL;
   if (NULL != m_swarm && m_swarm->gameMap())
   {
      int bestDist = std::numeric_limits<int>::max();

      std::vector< EntityBase* >::const_iterator targetIT (m_swarm->targets().begin()), end (m_swarm->targets().end());
      for ( ; targetIT != end; ++targetIT)
      {
         int distCost = m_swarm->gameMap()->getEstimatedDistanceCost(m_x, m_y, (*targetIT)->x(), (*targetIT)->y());
         if (distCost < getFleeRange () && distCost < bestDist) 
         {
            bestDist = distCost;
            bestTarget = (*targetIT);
         }
      }
   }

   return bestTarget;
}

bool HumanBase::hasWeapon () const
{
   bool bResult = false;
   switch (m_HumanType)
   {
   case kHumanType_Normal:
   case kHumanType_Pistol:
   case kHumanType_Shotgun:
   case kHumanType_MachineGun:
   case kHumanType_MechWarrior:
   case kHumanType_Tank:
   case kHumanType_Fat:
      bResult = true;
      break;
   }

   return bResult;
}

void HumanBase::idle ()
{
   m_bFleeing = false;
}

void HumanBase::flee (float timeSlice)
{
   if (NULL != m_swarm)
   {
      const EntityBase* enemy = getBestTarget ();      
      if (NULL != enemy)
      {
         m_bFleeing = true;

         Position refugePos;
         if (canBeMoved())
         {
            const int targetX = enemy->x();
            const int targetY = enemy->y();
            if (targetX != m_enemyX || targetY != m_enemyY || m_currentPath.empty())
            {
               m_enemyX = targetX;
               m_enemyY = targetY;
               m_swarm->gameMap()->getRefugePoint (Position(m_enemyX, m_enemyY), Position(m_x, m_y), refugePos);
               m_swarm->gameMap()->getPath(Position(m_x, m_y), refugePos, m_currentPath, mobileProperties().lineOfSight);
            }
            // move the entity towards the target
            if (m_currentPath.size() > 1) //don't move to the target position, entity should be next to the target to be able to attack it
            {
               const Position& pos = m_currentPath.front();
               if (m_swarm->gameMap()->moveEntity<MobileEntity>(this, pos.x(),pos.y()))
                  m_currentPath.pop_front();
               else
               {
                  m_swarm->gameMap()->getRefugePoint (Position(m_enemyX, m_enemyY), Position(m_x, m_y), refugePos);
                  m_swarm->gameMap()->getPath(Position(m_x, m_y), refugePos, m_currentPath, mobileProperties().lineOfSight);
               }
            }
         }
      }
   }
}

bool HumanBase::isMoving() const
{
   return state() == HumanState::kChase || state() == HumanState::kFlee;
}
