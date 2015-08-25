#ifndef _ZOMBIE_STATE_MACHINE_H__
#define _ZOMBIE_STATE_MACHINE_H__

#include "StateMachine.h"
#include <string>

class ZombieStateMachine;
class ZombieBase;

class ZombieState : public StateBase
{
public:

   static const std::string kIdle;
   static const std::string kChase;
   static const std::string kEatBody;
   static const std::string kAttack; 
   static const std::string kDead; 

   ZombieState(ZombieStateMachine* stateMachine, ZombieBase* zombie) : m_sm(stateMachine), m_zombie(zombie) {}
   ~ZombieState() {}

   bool idle();
   bool chase();
   bool eat_body();
   bool attack();
   bool dead();

   //TODO: do we need any generic method for each state e.g. game_clock_tick() ?

protected:
   ZombieStateMachine* m_sm;
   ZombieBase*         m_zombie;
};

class ZombieIdleState : public ZombieState
{
public:
   ZombieIdleState(ZombieStateMachine* sm, ZombieBase* zombie) : ZombieState(sm, zombie) {}
   void execute(float timeSlice);
};

class ZombieDeadState : public ZombieState
{
public:
   ZombieDeadState(ZombieStateMachine* sm, ZombieBase* zombie) : ZombieState(sm, zombie) {}
   void execute(float timeSlice);
};

class ZombieChaseState : public ZombieState
{
public:
   ZombieChaseState(ZombieStateMachine* sm, ZombieBase* zombie) : ZombieState(sm, zombie) {}
   void execute(float timeSlice);
};

class ZombieEatBodyState : public ZombieState
{
public:
   ZombieEatBodyState(ZombieStateMachine* sm, ZombieBase* zombie) : ZombieState(sm, zombie) {}
   void execute(float timeSlice);
};

class ZombieAttackState : public ZombieState
{
public:
   ZombieAttackState(ZombieStateMachine* sm, ZombieBase* zombie) : ZombieState(sm, zombie) {}
   void execute(float timeSlice);
};

class ZombieStateMachine : public StateMachine
{
public:
   ZombieStateMachine(ZombieBase* zombie);
};

#endif _ZOMBIE_STATE_MACHINE_H__