#ifndef _ZOMBIEBASE_H_
#define _ZOMBIEBASE_H_

#include "Entities/MobileEntity.h"
#include "StateMachine/ZombieStateMachine.h"
#include "Map/Position.h"

#include <list>

class ZombieBase : public MobileEntity
{
public:
    ZombieBase(unsigned long id, EntityType type);
    ~ZombieBase(void);

   void step(float timeSlice);
   bool isHuman () { return false; }
   bool isDead() const;
   bool isMoving() const;

   bool hasTargetsInLoS() const;   
   bool canEatBody() const;
  
   void eatBody();
   bool reachedTheTargetOfGod();
   void chaseTarget(float timeSlice);
};

#endif//_ZOMBIEBASE_H_