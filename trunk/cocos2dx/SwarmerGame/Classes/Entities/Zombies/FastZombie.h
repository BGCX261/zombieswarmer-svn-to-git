#ifndef _FAST_ZOMBIE_H__
#define _FAST_ZOMBIE_H__

#include "ZombieBase.h"
#include "../StateMachine/ZombieStateMachine.h"

//////////////////////////////////////////////////////////////////////////
class FastZombie : public ZombieBase
{
public:
	FastZombie (unsigned long id)
      : ZombieBase (id, EntityFastZombie)
   {
      setStateMachine (new ZombieStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeFists), MobileEntity::kWeapon_Default, -1);
   }
	~FastZombie () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_FAST_ZOMBIE_H__