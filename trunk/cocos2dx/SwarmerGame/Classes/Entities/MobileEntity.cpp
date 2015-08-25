#include "MobileEntity.h"
#include "Map/GameMap.h"
#include "Map/MapNode.h"
#include "Map/Position.h"
#include "Entities/EntitySwarm.h"
#include "Entities/xmlDataLoader.h"
#include <assert.h> 


MobileEntity::MobileEntity(unsigned long id, EntityType type,const MobileProperties& mp)
: EntityBase (id, type, true)
, m_isDangerous(false)
, m_moveTime(0)
, m_recoveryTime (0)
, m_enduranceTime (0)
, m_lastAttackTime(0)
, m_isTargetGodTarget(false)
, m_onlyWalk (false)
{   
   m_numAmmo[kWeapon_Default] = 0;
   m_numAmmo[kWeapon_AmmoDependent] = 0;
   
   if (!xmlDataManager::instance().getMobileEntityProperties(type, m_mobileProperties))
      m_mobileProperties = mp;
}

MobileEntity::~MobileEntity(void)
{
}

bool MobileEntity::hasMoved() const
{
   return !( m_lastX == m_x && m_lastY == m_y );
}

unsigned int MobileEntity::moveDirection() const
{
   unsigned int degrees = 0; //north
   if (hasLastPosition())
   {
      int dx = m_x - m_lastX;
      int dy = m_y - m_lastY;
      if (dx > 0)
      {
         if (dy < 0)
            degrees = 45;
         else if (dy == 0)
            degrees = 90;
         else
            degrees = 135;
      }
      else if (dx < 0)
      {
         if (dy < 0)
            degrees = 315;
         else if (dy == 0)
            degrees = 270;
         else
            degrees += 225;
      }
      else
      {
         if (dy > 0)
            degrees = 180;
      }
   }
   return degrees;
}

void MobileEntity::updateMobileProperties(float timeSlice)
{
   if (m_enduranceTime >= endurance())
   {
      if (m_recoveryTime < recovery())
         m_recoveryTime += timeSlice;
      else 
         m_enduranceTime = 0;
   }
   else
   {
      if (isMoving())
          m_enduranceTime += timeSlice;
      m_recoveryTime = 0;
   }
    
   m_moveTime += timeSlice;
}

void MobileEntity::moveTowardsTarget(EntityBase* target, int estimatedDistCost, float timeSlice)
{
   assert (NULL != m_swarm);
   m_isTargetGodTarget = target == m_swarm->targetOfGod();
   Position targetPos = Position(target->x(), target->y());
   setTargetPosition(targetPos);
   moveForward(targetPos, estimatedDistCost, timeSlice);
}

void MobileEntity::setTargetPosition(const Position& targetPos)
{
   if (targetPos != m_targetPos)
   {
      m_targetPos = targetPos;
      m_pathToTarget.clear();
      m_starNodes.reset();
   }
}

void MobileEntity::moveForward(const Position& targetPos, int estimatedDistCost, float timeSlice)
{
   GameMap* map = m_swarm->gameMap();
   if (NULL != map)
   {
      if (targetPos != m_targetPos)
      {
         setTargetPosition(targetPos);
      }

      if (canBeMoved())
      {
         if (m_pathToTarget.empty() )
         {
            //Each time slice we calculate a part of the path, for the final slice we have to make sure that we looked through all available nodes
            GameMap::PathSearchParams params;
            params.keepLineOfSight = !m_isTargetGodTarget;
            map->getPath(Position(m_x, m_y), m_targetPos, m_pathToTarget, &m_starNodes, StarNodes::kMaxNodes, mobileProperties().lineOfSight, params);
         }

         // move the entity towards the target
         if (m_pathToTarget.size() > 1) //don't move to the target position, entity should be next to the target to be able to attack it
         {
            const Position& pos = m_pathToTarget.front();
            if (map->moveEntity<MobileEntity>(this, pos.x(), pos.y() ))
            {
               m_pathToTarget.pop_front();
               m_moveTime = 0;
            }
            else
            {
               bool bForceUpdate = false;
               MapNode* nextNode = map->nodeAt(pos.x(), pos.y() );
               if (nextNode && nextNode->getMobileEntity())
               {
                  bForceUpdate = nextNode->getMobileEntity()->isHuman();
               }

               if (bForceUpdate || estimatedDistCost < map->getProximityDistanceThreshold() )
               {
                  //CCLOG("Below proximity threshold - optimizing path");
                  GameMap::PathSearchParams params;
                  params.keepLineOfSight = !m_isTargetGodTarget;
                  map->getPath(Position(m_x, m_y), m_targetPos, m_pathToTarget, mobileProperties().lineOfSight, params);
               }
            }
         }
              
         m_starNodes.reset();
      }
      else
      {
         if (m_pathToTarget.empty() )
         {
            //TODO: Take into account of map size, time slice and estimated distance cost
            int nodeSliceCount = 50;

/*
            //Threshold when we need to explore more nodes per time unit
            if (estimatedDistCost > map->getProximityDistanceThreshold() * 5 )
            {
               //How many nodes we should have explored by now (if we have to use all kMaxNodes)
               int nodeTargetSliceCount = StarNodes::kMaxNodes / ( walkSpeed() /  m_timeToMove) - m_starNodes.getCurrentNodeIndex();
               nodeSliceCount = nodeTargetSliceCount;
            }
*/

            GameMap::PathSearchParams params;
            params.keepLineOfSight = !m_isTargetGodTarget;
            map->getPath(Position(m_x, m_y), m_targetPos, m_pathToTarget, &m_starNodes, nodeSliceCount, mobileProperties().lineOfSight, params);
         }
      }
   }
}


float MobileEntity::moveSpeed() const
{
   if (m_enduranceTime < endurance() && !m_onlyWalk)
      return runSpeed();
   else return walkSpeed();
}

bool MobileEntity::canBeMoved () const
{
   return m_moveTime >= moveSpeed();
}

int MobileEntity::numAmmo () const
{
   int res = m_numAmmo[kWeapon_Default];

   if ( m_weapons[kWeapon_AmmoDependent].get() )
      res = m_numAmmo[kWeapon_AmmoDependent]; 

   return res;
}

void MobileEntity::setMobileProperties (const MobileProperties& mp)
{
   m_mobileProperties = mp;
}

void MobileEntity::attackTarget (float timeSlice)
{
   if (NULL != m_swarm)
   {
      m_lastAttackTime += timeSlice;

      if ( m_lastAttackTime >= getWeapon()->rateOfFire() )
      {
         int estimatedDistCost = 0;
         EntityBase* bestTarget = getBestTarget(estimatedDistCost);

         if (bestTarget != NULL && bestTarget != m_swarm->targetOfGod() && Position::inRange(Position(m_x, m_y), Position(bestTarget->x(), bestTarget->y()), getWeapon ()->range()))
         {
            MobileEntity *target = dynamic_cast<MobileEntity*>(bestTarget);
            if (NULL != target)
            {
                target->reduceHitPoints(Weapon::calculateDamage(getWeapon ().get()));
                m_numAmmo[kWeapon_AmmoDependent] = (m_numAmmo[kWeapon_AmmoDependent] > 0) ? m_numAmmo[kWeapon_AmmoDependent] - 1 : 0;
            }
         }

         m_lastAttackTime = 0.0f;
      }
   }
}

bool MobileEntity::targetInAttackRange () const
{
   bool bResult = false;
   if (NULL != m_swarm && m_swarm->gameMap())
   {
      std::vector< EntityBase* >::const_iterator it(m_swarm->targets().begin()), end(m_swarm->targets().end());
      for ( ; it != end; ++it)
      {
         assert (getWeapon ().get() != NULL); 
         unsigned int targetX = (*it)->x();
         unsigned int targetY = (*it)->y();
         if (m_numAmmo != 0 && Position::inRange(Position(m_x, m_y), Position(targetX, targetY), getWeapon ()->range()))
         {
            if (m_swarm->gameMap()->checkLineOfSight(Position(m_x, m_y), Position(targetX, targetY), mobileProperties().lineOfSight))
               return true;
         }
      }
   }
   return bResult;
}

void MobileEntity::setWeapon (std::shared_ptr<Weapon> w, EEntityWeapon t, int ammo)
{
   m_weapons[t] = w;
   m_numAmmo[t] = ammo;
}

EntityBase* MobileEntity::getBestTarget (int& estimatedDistCost) const
{
   const std::vector< EntityBase * >& targets = m_swarm->targets();
   std::vector< EntityBase* >::const_iterator targetIT = targets.begin(), targetLimit = targets.end();
   EntityBase* bestTarget = NULL;
   int bestDist = std::numeric_limits<int>::max();
   assert(NULL != m_swarm);
   GameMap* map = m_swarm->gameMap();
   if (NULL != map)
   {
      // determine which target to advance towards
      for ( ; targetIT != targetLimit; ++targetIT )
      {
         int distCost = map->getEstimatedDistanceCost(m_x, m_y, (*targetIT)->x(), (*targetIT)->y());
         if (*targetIT != m_swarm->targetOfGod() && distCost < bestDist && map->checkLineOfSight(Position(m_x, m_y), Position((*targetIT)->x(), (*targetIT)->y()), mobileProperties().lineOfSight))
         {
            bestDist = distCost;
            bestTarget = *targetIT;
            estimatedDistCost = bestDist;
         }
      }
   }
   //if no targets add target of god 
   if (NULL == bestTarget && NULL != m_swarm->targetOfGod())
   {
      bestTarget = m_swarm->targetOfGod();
      estimatedDistCost = map->getEstimatedDistanceCost(m_x, m_y, bestTarget->x(), bestTarget->y());
   }

   return bestTarget;
}


void MobileEntity::reduceHitPoints(int level)
{ 
   int reduced = m_mobileProperties.reduceHitPoints(level);
   m_mobileStats.m_lastReducedHitPoints += reduced;
}

const std::string& MobileEntity::state() const
{
   return stateMachine().state();
}

void MobileEntity::chaseTarget (float timeSlice)
{
   if (NULL != m_swarm)
   {
      int estimatedDistCost = 0;
      EntityBase* bestTarget = getBestTarget(estimatedDistCost);

      // for now, we assume that the speed are defined as pixels per second
      // progress the entity towards the best target
      if ( bestTarget )
      {
         moveTowardsTarget(bestTarget, estimatedDistCost, timeSlice);
      }
   }
}

