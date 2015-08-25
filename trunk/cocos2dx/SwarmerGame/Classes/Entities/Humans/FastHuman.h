#ifndef _FAST_HUMAN_H__
#define _FAST_HUMAN_H__

#include "HumanBase.h"
#include "StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kFastHumanHitPoints = 10;
static const float kFastHumanWalkSpeed = 0.04f;
static const float kFastHumanRunSpeed = 0.4f;
static const float   kFastHumanEndurance = 3;
static const float   kFastHumanRecovery = 1;

class FastHuman : public HumanBase
{
public:
	FastHuman (unsigned long id)
      : HumanBase (id, EntityFastHuman, MobileProperties (kFastHumanHitPoints, kLimitOfLineOfSight, kFastHumanWalkSpeed, kFastHumanRunSpeed, kFastHumanEndurance, kFastHumanRecovery))
   {
      setHumanType (kHumanType_Normal);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeHumanFists), MobileEntity::kWeapon_Default, -1);
   }
	~FastHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_FAST_HUMAN_H__