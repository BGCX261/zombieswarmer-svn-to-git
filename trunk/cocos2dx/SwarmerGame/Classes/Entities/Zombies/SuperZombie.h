#ifndef _SUPER_ZOMBIE_H__
#define _SUPER_ZOMBIE_H__

#include "ZombieBase.h"
#include "../StateMachine/ZombieStateMachine.h"

//////////////////////////////////////////////////////////////////////////

class SuperZombie : public ZombieBase
{
public:
	SuperZombie (unsigned long id)
      : ZombieBase (id, EntitySuperZombie)
   {
      setStateMachine (new ZombieStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeBigZombieFists), MobileEntity::kWeapon_Default, -1);
   }
	~SuperZombie () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_SUPER_ZOMBIE_H__