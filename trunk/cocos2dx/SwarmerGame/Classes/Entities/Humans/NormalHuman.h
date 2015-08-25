#ifndef _NORMAL_HUMAN_H__
#define _NORMAL_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kNormalHumanHitPoints = 10;
static const float kNormalHumanWalkSpeed = 0.02f;
static const float kNormalHumanRunSpeed = 0.15f;
static const float   kNormalHumanEndurance = 3.0f;
static const float   kNormalHumanRecovery = 1.0f;

class NormalHuman : public HumanBase
{
public:
	NormalHuman (unsigned long id)
      : HumanBase (id, EntityNormalHuman, MobileProperties (kNormalHumanHitPoints, kLimitOfLineOfSight, kNormalHumanWalkSpeed, kNormalHumanRunSpeed, kNormalHumanEndurance, kNormalHumanRecovery))
   {
      setHumanType (kHumanType_Normal);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeHumanFists), MobileEntity::kWeapon_Default, -1);
   }
	~NormalHuman () {}
};
//////////////////////////////////////////////////////////////////////////
#endif