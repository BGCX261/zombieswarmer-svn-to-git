#include <cmath>
#include <assert.h> 
#include "ZombieBase.h"
#include "Entities/EntitySwarm.h"
#include "Entities/Humans/HumanBase.h"
#include "Entities/Weapon.h"
#include "Map/Position.h"
#include "Map/MapNode.h"
#include "Map/GameMap.h"
#include "Entities/xmlDataLoader.h"

#include "cocos2d.h"
using namespace cocos2d;

#ifndef M_PI
   #define M_PI       3.14159265358979323846
#endif//M_PI

#ifdef min
   #undef min
#endif

#ifdef max
   #undef max
#endif

static const int kZombieHitPoints = 25;
static const float kZombieWalkSpeed = 0.4f;
static const float kZombieRunSpeed = 0.2f;
static const float   kZombieEndurance = 1.0f;
static const float   kZombieRecovery = 1.0f;


ZombieBase::ZombieBase(unsigned long id, EntityType type)
: MobileEntity(id, type, MobileProperties(kZombieHitPoints, kLimitOfLineOfSight, kZombieWalkSpeed, kZombieRunSpeed, kZombieEndurance, kZombieRecovery))
{
   //setStateMachine(new ZombieStateMachine(this));
   setWeapon(WeaponFactory::GetWeapon(WeaponTypeFists), MobileEntity::kWeapon_Default, -1);
}

////////////////////////////////////////////////////////////////////////////////

ZombieBase::~ZombieBase(void)
{
}

////////////////////////////////////////////////////////////////////////////////

void ZombieBase::step(float timeSlice)
{
   if (NULL != m_swarm)
   {
      stateMachine()()->execute(timeSlice);
   }
}

////////////////////////////////////////////////////////////////////////////////

bool ZombieBase::hasTargetsInLoS() const
{
   bool bResult = false;
   if (NULL != m_swarm && m_swarm->gameMap())
   {
      GameMap* map = m_swarm->gameMap();
      std::vector< EntityBase* >::const_iterator it(m_swarm->targets().begin()), end(m_swarm->targets().end());
      int targetX = -1, targetY = -1;
      for ( ; it != end; ++it)
      {
         targetX = (*it)->x();
         targetY = (*it)->y();
         if (map->checkLineOfSight(Position(m_x, m_y), Position(targetX, targetY), mobileProperties().lineOfSight))
            bResult = true;;
      }

      //if (!bResult && NULL != m_swarm->targetOfGod())
      //   bResult = true;
   }
   return bResult; 
}


////////////////////////////////////////////////////////////////////////////////


bool ZombieBase::canEatBody() const
{
    if (NULL != m_swarm)
    {
        std::vector< EntityBase * >& targets = m_swarm->targets();
        std::vector< EntityBase* >::iterator targetIT = targets.begin();
        std::vector< EntityBase* >::const_iterator targetLimit = targets.end();
    
        for ( ; targetIT != targetLimit; ++targetIT )
        {
           //TODO: provide something like EatableEntity ?
           if ((*targetIT)->isHuman())
           {
              unsigned int targetX = (*targetIT)->x();
              unsigned int targetY = (*targetIT)->y();
              HumanBase* human = static_cast<HumanBase*>(*targetIT);
              if (human->isDead() && Position::inRange(Position(m_x, m_y), Position(targetX, targetY), 1) ) //TODO: 1 means next to body
                 return true;
           }
        }

    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void ZombieBase::eatBody()
{
    if (NULL != m_swarm)
    {
        std::vector< EntityBase * >& targets = m_swarm->targets();
        std::vector< EntityBase* >::iterator targetIT = targets.begin();
        std::vector< EntityBase* >::const_iterator targetLimit = targets.end();
    
        for ( ; targetIT != targetLimit; ++targetIT )
        {
           if ((*targetIT)->isHuman())
           {
              unsigned int targetX = (*targetIT)->x();
              unsigned int targetY = (*targetIT)->y();
              HumanBase* human = static_cast<HumanBase*>(*targetIT); //TODO: need to be changed when we have more than one entity that can be bitten
              if (human->isDead() && Position::inRange(Position(m_x, m_y), Position(targetX, targetY), 1) ) //TODO: 1 means next to body
              {
                  human->bite();
                  break;
              }
           }
        }
    }
}

bool ZombieBase::isDead () const
{
   return stateMachine().state() == ZombieState::kDead;
}

bool ZombieBase::reachedTheTargetOfGod()
{
   int estimatedDistCost = 0;
   EntityBase* bestTarget = getBestTarget(estimatedDistCost);
   assert (NULL != m_swarm);
   assert (NULL != m_swarm->gameMap());
   bool bUpdatePath = false;
   if (NULL != bestTarget && bestTarget == m_swarm->targetOfGod())
   {
      if (Position::inRange(Position(m_x, m_y), Position(bestTarget->x(), bestTarget->y()), 1))
         return true;

      const std::list<Position> path = pathToTarget();

      if (path.empty() && !m_targetPos.isValid() )
         return false;

      if (!path.empty())
      {
         if ((path.back().x() != bestTarget->x() || path.back().y() != bestTarget->y()))
            return false;

         Position pos(path.front());

         int x = pos.x();
         int y = pos.y();
         MapNode* node = m_swarm->gameMap()->nodeAt(x, y);
         assert (NULL != node);

         if (node->getMobileEntity() != NULL && !node->getMobileEntity()->isHuman())
            bUpdatePath = true;
      }
      else
      {
         bUpdatePath = true;
      }

      if (bUpdatePath)
      {
         int nodeSliceCount = 50;
         GameMap::PathSearchParams params;
         params.keepLineOfSight = false;
         m_swarm->gameMap()->getPath(Position(m_x, m_y), m_targetPos, m_pathToTarget, &m_starNodes, nodeSliceCount, mobileProperties().lineOfSight, params);
         if (m_moveTime >= walkSpeed())
         {
            m_moveTime = 0;
            m_starNodes.reset();
         }
         return true;
      }

   }
   return false;
}

bool ZombieBase::isMoving() const
{
   return state() == ZombieState::kChase;
}


void ZombieBase::chaseTarget (float timeSlice)
{
	if (NULL != m_swarm)
	{
		int estimatedDistCost = 0;
		EntityBase* bestTarget = getBestTarget(estimatedDistCost);

      setOnlyWalk(bestTarget == m_swarm->targetOfGod());
		
		if ( bestTarget )
		{
			moveTowardsTarget(bestTarget, estimatedDistCost, timeSlice);
		}
	}
}