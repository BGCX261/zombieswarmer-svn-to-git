#ifndef _STATIC_ENTITY_H_
#define _STATIC_ENTITY_H_

#include "EntityBase.h"

template <EntityType TEntityType>
class StaticEntity : public EntityBase
{
public:
    StaticEntity(unsigned long id);
    ~StaticEntity(void);

    void step(float timeSlice);
    bool isDead () const;
};

////////////// TEMPLATE IMPLEMENTATION //////////////////

template <EntityType TEntityType>
StaticEntity<TEntityType>::StaticEntity(unsigned long id)
: EntityBase(id, TEntityType, false)
{
}

template <EntityType TEntityType>
StaticEntity<TEntityType>::~StaticEntity()
{
}

template <EntityType TEntityType>
void StaticEntity<TEntityType>::step(float timeSlice)
{
}

#endif//_STATIC_ENTITY_H_