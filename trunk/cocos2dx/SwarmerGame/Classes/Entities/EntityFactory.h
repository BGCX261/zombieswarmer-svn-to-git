#ifndef _ENTITY_FACTORY_H_
#define _ENTITY_FACTORY_H_

#include "EntityBase.h"
#include "ContainerEntity.h"

class EntityFactory
{
    EntityFactory(void);
    ~EntityFactory(void);

public:

    static EntityFactory& instance();

    template <class EntityType>
    EntityBase* createEntity();
    
    template <class ContainerType>
    EntityBase* createEntity(EntityType type);

    EntityBase* createEntity (EntityType type);

private:

    unsigned long           m_lastEntityID;
};

///////// Template implementation ////////////

template <class EntityType>
EntityBase* EntityFactory::createEntity()
{
    EntityType* obj = new EntityType(m_lastEntityID+1);
    EntityBase* entity = dynamic_cast< EntityBase* >(obj);

    if ( entity )
    {
        m_lastEntityID++;
    }
    else
    {
        delete obj;
    }

    return entity;
}

template <class ContainerType>
EntityBase* EntityFactory::createEntity( EntityType type )
{
   EntityBase* entity = new ContainerType(m_lastEntityID+1, type);

   if ( entity )
   {
      m_lastEntityID++;
   }
   else
   {
      delete entity;
   }

   return entity;
}

#endif//_ENTITY_FACTORY_H_