#ifndef _FAT_ZOMBIE_H__
#define _FAT_ZOMBIE_H__

#include "ZombieBase.h"
#include "../StateMachine/ZombieStateMachine.h"

//////////////////////////////////////////////////////////////////////////
class FatZombie : public ZombieBase
{
public:
	FatZombie (unsigned long id)
      : ZombieBase (id, EntityFatZombie)
   {
      setStateMachine (new ZombieStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeFists), MobileEntity::kWeapon_Default, -1);
   }
	~FatZombie () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_FAT_ZOMBIE_H__