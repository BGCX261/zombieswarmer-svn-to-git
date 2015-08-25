#ifndef _SHOTGUN_HUMAN_H__
#define _SHOTGUN_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kShotGunHumanHitPoints = 10;
static const float kShotGunHumanWalkSpeed = 0.01f;
static const float kShotGunHumanRunSpeed = 0.1f;
static const float   kShotGunHumanEndurance = 3;
static const float   kShotGunHumanRecovery = 1;

class ShotGunHuman : public HumanBase
{
public:
	ShotGunHuman (unsigned long id)
      : HumanBase (id, EntityShotgunHuman, MobileProperties (kShotGunHumanHitPoints, kLimitOfLineOfSight, kShotGunHumanWalkSpeed, kShotGunHumanRunSpeed, kShotGunHumanEndurance, kShotGunHumanRecovery))
   {
      setHumanType (kHumanType_Shotgun);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeShotgun), MobileEntity::kWeapon_AmmoDependent, 10);
   }
	~ShotGunHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_SHOTGUN_HUMAN_H__