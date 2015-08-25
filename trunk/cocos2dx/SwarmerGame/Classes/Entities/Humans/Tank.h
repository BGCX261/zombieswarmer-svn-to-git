#ifndef _TANK_H__
#define _TANK_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kTankHitPoints = 100;
static const float kTankWalkSpeed = 0.02f;
static const float kTankRunSpeed = 0.15f;
static const float   kTankEndurance = 3;
static const float   kTankRecovery = 1;

class Tank : public HumanBase
{
public:
	Tank (unsigned long id)
      : HumanBase (id, EntityTank, MobileProperties (kTankHitPoints, kLimitOfLineOfSight, kTankWalkSpeed, kTankRunSpeed, kTankEndurance, kTankRecovery))
   {
      setHumanType (kHumanType_Normal);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeCannon), MobileEntity::kWeapon_AmmoDependent, 10);
   }
	~Tank () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_TANK_H__