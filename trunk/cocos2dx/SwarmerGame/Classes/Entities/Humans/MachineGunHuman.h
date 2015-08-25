#ifndef _MACHINEGUN_HUMAN_H__
#define _MACHINEGUN_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kMachinGunHumanHitPoints = 10;
static const float kMachinGunHumanWalkSpeed = 0.01f;
static const float kMachinGunHumanRunSpeed = 0.1f;
static const float   kMachinGunHumanEndurance = 3;
static const float   kMachinGunHumanRecovery = 1;

class MachineGunHuman : public HumanBase
{
public:
	MachineGunHuman (unsigned long id)
      : HumanBase (id, EntityMachineGunHuman, MobileProperties (kMachinGunHumanHitPoints, kLimitOfLineOfSight, kMachinGunHumanWalkSpeed, kMachinGunHumanRunSpeed, kMachinGunHumanEndurance, kMachinGunHumanRecovery))
   {
      setHumanType (kHumanType_MachineGun);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeMachineGun), MobileEntity::kWeapon_AmmoDependent, 100);
   }
	~MachineGunHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_MACHINEGUN_HUMAN_H__