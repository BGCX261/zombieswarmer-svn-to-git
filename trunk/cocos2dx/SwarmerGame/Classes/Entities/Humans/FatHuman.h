#ifndef _FAT_HUMAN_H__
#define _FAT_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kFatHumanHitPoints = 50;
static const float kFatHumanWalkSpeed = 0.01f;
static const float kFatHumanRunSpeed = 0.1f;
static const float   kFatHumanEndurance = 2.0f;
static const float   kFatHumanRecovery = 5.0f;

class FatHuman : public HumanBase
{
public:
	FatHuman (unsigned long id)
      : HumanBase (id, EntityFatHuman, MobileProperties (kFatHumanHitPoints, kLimitOfLineOfSight, kFatHumanWalkSpeed, kFatHumanRunSpeed, kFatHumanEndurance, kFatHumanRecovery))
   {
      setHumanType (kHumanType_Fat);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon(WeaponFactory::GetWeapon(WeaponTypeHumanFatFists), MobileEntity::kWeapon_Default, -1);
   }
	~FatHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif // _FAT_HUMAN_H__