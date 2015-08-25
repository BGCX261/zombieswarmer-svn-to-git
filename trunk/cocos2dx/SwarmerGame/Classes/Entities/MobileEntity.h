#ifndef _MOBILEENTITY_H_
#define _MOBILEENTITY_H_

#include "EntityBase.h"
#include "Map/StarNode.h"
#include "Map/Position.h"
#include "StateMachine/StateMachine.h"
#include "Weapon.h"
#include "MobileProperties.h"
#include <list>


class Position;

class MobileEntity;
class MobileStats
{
public:
   void reset() { m_lastReducedHitPoints = 0; }
   int  lastReducedHitPoints() const           { return m_lastReducedHitPoints; }

private:
   friend MobileEntity;
   MobileStats () : m_lastReducedHitPoints(0) {}

   int            m_lastReducedHitPoints; // last decreased amount of points
};


class MobileEntity : public EntityBase
{
public:
  
   enum EEntityWeapon
   {
      kWeapon_Default = 0,
      kWeapon_AmmoDependent = 1
   };

   MobileEntity(unsigned long id, EntityType type,const MobileProperties& mp);
   ~MobileEntity(void);

   bool canBeMoved () const;
   bool isDangerous() const                    { return m_isDangerous; }
   int numAmmo() const;

   bool hasMoved() const;
   /* Return the direction of the vector in which an entity is moving (in degrees), starting with 0 degrees on the north and clockwise direction */
   unsigned int moveDirection() const;

   virtual bool isDead() const = 0;                         
   virtual bool isMoving() const = 0;                         

   void setMobileProperties (const MobileProperties& mp);
   void updateMobileProperties (float timeSlice);
   MobileProperties& mobileProperties()  { return m_mobileProperties; }
   const MobileProperties& mobileProperties() const  { return m_mobileProperties; }


   float moveSpeed() const;

   void moveTowardsTarget(EntityBase* target, int estimatedDistCost, float timeSlice);
   void moveForward(const Position& targetPos, int estimatedDistCost, float timeSlice);

   
   void setWeapon (std::shared_ptr<Weapon> w, EEntityWeapon t, int ammo = 0);
   inline std::shared_ptr<Weapon> getWeapon () const { return (m_numAmmo[kWeapon_AmmoDependent] > 0) ? m_weapons[kWeapon_AmmoDependent] : m_weapons[kWeapon_Default]; }

   int hitPoints() const                       { return m_mobileProperties.hitPoints; }
   void reduceHitPoints(int level);

   const MobileStats& mobileStats() const      { return m_mobileStats; }
   MobileStats& mobileStats()                  { return m_mobileStats; }

   inline void setStateMachine(StateMachine* sm)      { m_sm.reset(sm); }

   inline StateMachine& stateMachine()                      { return *(m_sm.get()); }
   inline const StateMachine& stateMachine() const          { return *(m_sm.get()); }

   void attackTarget(float timeSlice);
   void chaseTarget(float timeSlice); 

   bool targetInAttackRange() const;
   EntityBase* getBestTarget(int& estimatedDistCost) const;


   bool resetLastAttackTime()                   { m_lastAttackTime = 0.0f;  return true; }

   /* Returns the current state of the entity. 
      It should be used compared to the specific entity's states as e.g. Human vs Zombie differs
   */
   const std::string& state() const;

protected:
   const std::list<Position>& pathToTarget() const { return m_pathToTarget; }
   void setTargetPosition(const Position& targetPos);

   /* Returns the move speed. Depends on entity endurance, recover, walk and run speed */
   float walkSpeed() const                     { return m_mobileProperties.walkSpeed; }
   float runSpeed() const                      { return m_mobileProperties.runSpeed; }
   float endurance() const                     { return m_mobileProperties.endurance; }
   float recovery() const                      { return m_mobileProperties.recovery; }
   bool onlyWalk() const                       { return m_onlyWalk; }
   void setOnlyWalk(bool walk)                 { m_onlyWalk = walk; }

   Position m_targetPos;
   std::list<Position> m_pathToTarget;
   // keeps the cumulative amount of time to move
   StarNodes           m_starNodes;
   float               m_moveTime;

private:

   MobileProperties    m_mobileProperties;

   MobileStats         m_mobileStats;

   // flag indicating whether this entity is to be considered dangerous
   bool                m_isDangerous;

   bool                m_onlyWalk;

   
   float               m_recoveryTime;
   float               m_enduranceTime;

   float               m_lastAttackTime;
   
   bool m_isTargetGodTarget;
   
   
   //m_weapons stores default weapon on fists and any per-unit weapon e.g. shotgun.
   std::shared_ptr<Weapon> m_weapons[2];
   // how many shots the weapon can fire before it runs out of ammo
   int                 m_numAmmo[2];

   std::shared_ptr<StateMachine> m_sm;
};

#endif//_MOBILEENTITY_H_