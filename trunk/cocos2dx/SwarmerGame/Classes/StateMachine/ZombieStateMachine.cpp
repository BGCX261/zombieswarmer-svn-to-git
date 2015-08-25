#include "ZombieStateMachine.h"
#include "Entities/Zombies/ZombieBase.h"
#include <assert.h>

const std::string ZombieState::kIdle            = "idle";
const std::string ZombieState::kChase           = "chase";
const std::string ZombieState::kEatBody         = "eat_body";
const std::string ZombieState::kAttack          = "attack";
const std::string ZombieState::kDead            = "dead";

bool ZombieState::idle()
{
   return m_sm->transition(kIdle);
}

bool ZombieState::chase()
{
   return m_sm->transition(kChase);
}

bool ZombieState::eat_body()
{
   return m_sm->transition(kEatBody);
}

bool ZombieState::attack()
{
   return m_sm->transition(kAttack);
}

bool ZombieState::dead ()
{
   return m_sm->transition(kDead);
}

//////////////////////////////////////////////////////////////////////////

void ZombieIdleState::execute(float timeSlice)
{
   assert (m_zombie != NULL);
   if (m_zombie->hitPoints() == kDestroyed)
      dead();

   if (m_zombie->hasTargetsInLoS() || !m_zombie->reachedTheTargetOfGod())
      chase();
}

void ZombieChaseState::execute(float timeSlice)
{
   assert (m_zombie != NULL);
   if (m_zombie->hitPoints() == kDestroyed)
      dead();

   if (!m_zombie->hasTargetsInLoS() && m_zombie->reachedTheTargetOfGod())
      idle();
   else if (m_zombie->targetInAttackRange())
      attack();
   else
      m_zombie->chaseTarget(timeSlice);
}



void ZombieDeadState::execute (float timeSlice)
{
   assert (m_zombie != NULL);

}

void ZombieAttackState::execute(float timeSlice)
{
   assert (m_zombie != NULL);
   if (m_zombie->hitPoints() == kDestroyed)
      dead();

   if (!m_zombie->targetInAttackRange())
      chase();
   else if (m_zombie->canEatBody())
      eat_body();
   else
      m_zombie->attackTarget(timeSlice);
}

void ZombieEatBodyState::execute(float timeSlice)
{
   assert (m_zombie != NULL);
   if (m_zombie->hitPoints() == kDestroyed)
      dead();

   if (!m_zombie->canEatBody())
      chase();
   else
      m_zombie->eatBody();
}

//////////////////////////////////////////////////////////////////////////

ZombieStateMachine::ZombieStateMachine(ZombieBase* zombie)
{
   createState(ZombieState::kIdle,  new ZombieIdleState(this, zombie));
   createState(ZombieState::kChase, new ZombieChaseState(this, zombie));
   createState(ZombieState::kEatBody, new ZombieEatBodyState(this, zombie));
   createState(ZombieState::kAttack, new ZombieAttackState(this, zombie));
   createState(ZombieState::kDead, new ZombieDeadState(this, zombie));

   setTransition(ZombieState::kIdle, ZombieState::kChase);
   setTransition(ZombieState::kIdle, ZombieState::kDead);
   
   setTransition(ZombieState::kChase, ZombieState::kIdle);
   setTransition(ZombieState::kChase, ZombieState::kAttack);
   setTransition(ZombieState::kChase, ZombieState::kDead);
   
   setTransition(ZombieState::kAttack, ZombieState::kEatBody, new TransitionOperation<MobileEntity>(zombie, &ZombieBase::resetLastAttackTime));
   setTransition(ZombieState::kAttack, ZombieState::kChase, new TransitionOperation<MobileEntity>(zombie, &ZombieBase::resetLastAttackTime));
   setTransition(ZombieState::kAttack, ZombieState::kDead, new TransitionOperation<MobileEntity>(zombie, &ZombieBase::resetLastAttackTime));

   setTransition(ZombieState::kEatBody, ZombieState::kChase);
   setTransition(ZombieState::kEatBody, ZombieState::kDead);   

   setState(ZombieState::kIdle);

}