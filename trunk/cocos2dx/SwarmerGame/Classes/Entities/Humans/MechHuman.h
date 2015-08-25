#ifndef _MECH_HUMAN_H__
#define _MECH_HUMAN_H__

#include "HumanBase.h"
#include "../StateMachine/HumanStateMachine.h"

//////////////////////////////////////////////////////////////////////////
static const int   kMechHumanHitPoints = 50;
static const float kMechHumanWalkSpeed = 0.03f;
static const float kMechHumanRunSpeed = 0.3f;
static const float   kMechHumanEndurance = 10;
static const float   kMechHumanRecovery = 1;

class MechHuman : public HumanBase
{
public:
	MechHuman (unsigned long id)
      : HumanBase (id, EntityMechHuman, MobileProperties (kMechHumanHitPoints, kLimitOfLineOfSight, kMechHumanWalkSpeed, kMechHumanRunSpeed, kMechHumanEndurance, kMechHumanRecovery))
   {
      setHumanType (kHumanType_MechWarrior);
      setStateMachine (new NormalHumanStateMachine(this));
      setWeapon (WeaponFactory::GetWeapon(WeaponTypeMechFists), MobileEntity::kWeapon_Default, -1);
   }
	~MechHuman () {}

private:
};
//////////////////////////////////////////////////////////////////////////
#endif //_MECH_HUMAN_H__