#ifndef _HUMANBASE_H_
#define _HUMANBASE_H_

#include <list>
#include "Entities/MobileEntity.h"
#include "Entities/Weapon.h"
#include "Map/Position.h"
#include "StateMachine/HumanStateMachine.h"

enum EHumanTypes
{
   kHumanType_Fat,
   kHumanType_Normal,
   kHumanType_Fast,
   kHumanType_Pistol,
   kHumanType_Shotgun,
   kHumanType_MachineGun,
   kHumanType_MechWarrior,
   kHumanType_Tank
};



class HumanBase : public MobileEntity
{
public:
    HumanBase (unsigned long id, EntityType type, MobileProperties mp);
    ~HumanBase ();

   void step(float timeSlice);
   bool isHuman () { return true; }
   bool isDead() const;
   bool isMoving() const;
  
   bool hasTargetsInLoS () const;
   bool targetInFleeRange () const;

   bool hasWeapon () const;
   const EntityBase* getBestTarget () const;

   bool isFleeing () const    { return m_bFleeing; }
   void setFleeing (bool val) { m_bFleeing = val; }
   int getFleeRange () const { return (m_bFleeing ? m_endFleeRange : m_beginFleeRange); }

   bool isBitten() const;
   /* Returns true when the human was bitten and is ready to be transformed to zombie */
   bool isZombie() const;

   void flee (float timeSlice);
   void idle ();
   void bite ()  { m_bBitten = true; }

   EHumanTypes getHumanType () const { return m_HumanType; }
   void setHumanType (EHumanTypes val) { m_HumanType = val; }

private:
    EHumanTypes m_HumanType;
    
    // human may flee zombies within x until y distance is between them.
    int m_beginFleeRange;
    int m_endFleeRange;
    bool m_bFleeing;
    bool m_bBitten;

    // when attacking or fleeing an entity, the human will cache their location and path
    int m_enemyX, m_enemyY;
    std::list<Position> m_currentPath;
    
    // Only used by weaponised entities
    std::vector<Position> m_AmmoPotLocations;
};


#endif//_ZOMBIEBASE_H_