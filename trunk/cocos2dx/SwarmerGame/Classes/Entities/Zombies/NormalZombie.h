#ifndef _NORMAL_ZOMBIE_H__
#define _NORMAL_ZOMBIE_H__

#include "ZombieBase.h"
#include "../StateMachine/ZombieStateMachine.h"
//////////////////////////////////////////////////////////////////////////

class NormalZombie : public ZombieBase
{
public:
	NormalZombie (unsigned long id)
      : ZombieBase (id, EntityNormalZombie)
   {
      setStateMachine (new ZombieStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeFists), MobileEntity::kWeapon_Default, -1);
   }
	~NormalZombie () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_NORMAL_ZOMBIE_H__