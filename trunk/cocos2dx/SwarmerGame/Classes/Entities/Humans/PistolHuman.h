#ifndef _PISTOL_HUMAN_H__
#define _PISTOL_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kPistolHumanHitPoints = 10;
static const float kPistolHumanWalkSpeed = 0.02f;
static const float kPistolHumanRunSpeed = 0.15f;
static const float   kPistolHumanEndurance = 3;
static const float   kPistolHumanRecovery = 1;

class PistolHuman : public HumanBase
{
public:
	PistolHuman (unsigned long id)
      : HumanBase (id, EntityPistolHuman, MobileProperties (kPistolHumanHitPoints, kLimitOfLineOfSight, kPistolHumanWalkSpeed, kPistolHumanRunSpeed, kPistolHumanEndurance, kPistolHumanRecovery))
   {
      setHumanType (kHumanType_Pistol);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypePistol), MobileEntity::kWeapon_AmmoDependent, 10);
   }
	~PistolHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_PISTOL_HUMAN_H__