#include "HumanStateMachine.h"
#include "Entities/Humans/HumanBase.h"
#include <assert.h>


const std::string HumanState::kIdle            = "idle";
const std::string HumanState::kChase           = "chase";
const std::string HumanState::kFlee            = "flee";
const std::string HumanState::kGatherAmmo      = "gather_ammo";
const std::string HumanState::kAttack          = "attack";
const std::string HumanState::kDead            = "dead";
const std::string HumanState::kBitten          = "bitten";

bool HumanState::idle ()
{
   return m_sm->transition(kIdle);
}

bool HumanState::flee ()
{
   return m_sm->transition(kFlee);
}

bool HumanState::bitten ()
{
   return m_sm->transition(kBitten);
}

bool HumanState::dead ()
{
   return m_sm->transition(kDead);
}

bool HumanState::chase ()
{
   return m_sm->transition(kChase);
}

bool HumanState::gather_ammo ()
{
   return m_sm->transition(kGatherAmmo);
}

bool HumanState::attack ()
{
   return m_sm->transition(kAttack);
}

//////////////////////////////////////////////////////////////////////////


void HumanIdleState::execute (float timeSlice)
{
    assert (m_human != NULL);
    if (m_human->hitPoints() == kDestroyed)
       dead();
    if (m_human->hasWeapon () &&  m_human->hasTargetsInLoS () && m_human->targetInAttackRange())
       attack ();
    else if (m_human->hasTargetsInLoS () && m_human->targetInFleeRange ())
       flee ();
}

void HumanFleeState::execute (float timeSlice)
{
   assert (m_human != NULL);
   if (m_human->hitPoints() == kDestroyed)
      dead();
   
   if (m_human->hasWeapon () &&  m_human->hasTargetsInLoS () && m_human->targetInAttackRange())
      attack ();
   else if (m_human->hasTargetsInLoS () && m_human->targetInFleeRange ())
      m_human->flee (timeSlice);
   else 
      idle ();      
}

void HumanDeadState::execute (float timeSlice)
{
   assert (m_human != NULL);
   if (m_human->isBitten())
      bitten ();      
}

void HumanBittenState::execute (float timeSlice)
{
   assert (m_human != NULL);
}

void HumanChaseState::execute (float timeSlice)
{
   assert (m_human != NULL);
   if (m_human->hitPoints() == kDestroyed)
      dead();
   if (!m_human->hasWeapon () || m_human->numAmmo() == 0)
      flee ();
   else if (m_human->targetInAttackRange())
      attack();
   else
      m_human->chaseTarget(timeSlice);

}

void HumanAttackState::execute (float timeSlice)
{
   assert (m_human != NULL);
   if (m_human->hitPoints() == kDestroyed)
      dead();

   if (!m_human->hasWeapon () || m_human->numAmmo() == 0)
      flee ();
   else if (!m_human->targetInAttackRange())
      chase();
   else
      m_human->attackTarget(timeSlice);
}

void HumanGatherAmmoState::execute (float timeSlice)
{
   assert (m_human != NULL);
}

//////////////////////////////////////////////////////////////////////////

HumanStateMachine::HumanStateMachine (HumanBase* human)
{
   createState(HumanState::kIdle,  new HumanIdleState(this, human));
   createState(HumanState::kFlee, new HumanFleeState(this, human));
   createState(HumanState::kDead, new HumanDeadState(this, human));
   createState(HumanState::kBitten, new HumanBittenState(this, human));

   setTransition(HumanState::kIdle, HumanState::kFlee);
   setTransition(HumanState::kFlee, HumanState::kIdle);
   setTransition(HumanState::kIdle, HumanState::kDead);
   setTransition(HumanState::kFlee, HumanState::kDead);
   setTransition(HumanState::kDead, HumanState::kBitten);

   setState(HumanState::kIdle);
}

//////////////////////////////////////////////////////////////////////////
NormalHumanStateMachine::NormalHumanStateMachine(HumanBase* human)
   : HumanStateMachine(human)
{
   createState(HumanState::kAttack, new HumanAttackState(this, human));

   setTransition(HumanState::kIdle, HumanState::kAttack);
   setTransition(HumanState::kFlee, HumanState::kAttack);

   setTransition(HumanState::kAttack, HumanState::kIdle);
   setTransition(HumanState::kAttack, HumanState::kFlee);
   setTransition(HumanState::kAttack, HumanState::kDead);
   setTransition(HumanState::kAttack, HumanState::kChase);

   setState(HumanState::kIdle);
}
