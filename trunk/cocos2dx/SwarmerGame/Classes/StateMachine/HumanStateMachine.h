#ifndef _HUMAN_STATE_MACHINE_H__
#define _HUMAN_STATE_MACHINE_H__

#include "StateMachine.h"
#include <string>

class HumanStateMachine;
class HumanBase;

class HumanState : public StateBase
{
public:

   static const std::string kIdle;
   static const std::string kChase;
   static const std::string kFlee;           
   static const std::string kGatherAmmo;
   static const std::string kAttack;
   static const std::string kDead;         
   static const std::string kBitten;

   HumanState(HumanStateMachine* stateMachine, HumanBase* human) : m_sm(stateMachine), m_human(human) {}
   ~HumanState() {}

   bool idle();
   bool flee ();
   bool dead ();
   bool bitten ();
   bool chase ();
   bool gather_ammo ();
   bool attack ();
   
   //TODO: do we need any generic method for each state e.g. game_clock_tick() ?

protected:
   HumanStateMachine* m_sm;
   HumanBase*         m_human;
};

//////////////////////////////////////////////////////////////////////////

class HumanIdleState : public HumanState
{
public:
   HumanIdleState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};


class HumanFleeState : public HumanState
{
public:
   HumanFleeState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

class HumanDeadState : public HumanState
{
public:
   HumanDeadState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

class HumanBittenState : public HumanState
{
public:
   HumanBittenState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

class HumanChaseState : public HumanState
{
public:
   HumanChaseState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

class HumanAttackState : public HumanState
{
public:
   HumanAttackState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

class HumanGatherAmmoState : public HumanState
{
public:
   HumanGatherAmmoState(HumanStateMachine* sm, HumanBase* Human) : HumanState(sm, Human) {}
   void execute(float timeSlice);
};

//////////////////////////////////////////////////////////////////////////

class HumanStateMachine : public StateMachine
{
public:
   HumanStateMachine(HumanBase* human);
};

//////////////////////////////////////////////////////////////////////////
class NormalHumanStateMachine : public HumanStateMachine 
{
public:
   NormalHumanStateMachine (HumanBase* human);
};
//////////////////////////////////////////////////////////////////////////


#endif _HUMAN_STATE_MACHINE_H__