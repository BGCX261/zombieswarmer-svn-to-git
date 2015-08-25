#ifndef _MAPNODE_H_
#define _MAPNODE_H_


#include "../Entities/StaticEntity.h"
#include "../Entities/ContainerEntity.h"
#include "../Entities/MobileEntity.h"
#include <cassert>

class EntityBase;
class MobileEntity;

class MapNode
{
public:
    MapNode(EntityBase* stationaryEntity = 0);
    ~MapNode(void);

    EntityBase* getStationaryEntity() const             { return m_stationaryEntity; }
    MobileEntity* getMobileEntity() const               { return m_mobileEntity; }
    EntityBase* getContainerEntity() const              { return m_containerEntity; }

    template <class EntityClass>
    bool setEntity(EntityClass* entity);

    void removeEntity(EntityBase* entity);

    bool isBlocking() const;
    bool isEmpty() const;
    void clear();

private:
    EntityBase*         m_stationaryEntity;
    EntityBase*         m_containerEntity;
    MobileEntity*       m_mobileEntity;
};

template <class EntityClass>
inline bool MapNode::setEntity (EntityClass* entity)
{
   assert(entity != NULL);
   assert(!entity->isMoveable() );

   if (m_stationaryEntity != NULL)
      return false;

   m_stationaryEntity = entity;
   return true;
}

template<>
inline bool MapNode::setEntity<MobileEntity>(MobileEntity* entity)
{
   assert(m_mobileEntity == NULL);

   m_mobileEntity = static_cast<MobileEntity*>(entity);
   return true;
}

template<>
inline bool MapNode::setEntity<ContainerEntity>(ContainerEntity* entity)
{
   if (m_containerEntity != NULL)
      return false;

   m_containerEntity = entity;
   return true;
}

#endif//_MAPNODE_H_