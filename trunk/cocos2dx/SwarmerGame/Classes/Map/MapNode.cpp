#include "MapNode.h"
#include "../Entities/EntityBase.h"
#include "../Entities/MobileEntity.h"
#include "../Entities/ContainerEntity.h"

MapNode::MapNode(EntityBase* stationaryEntity)
: m_stationaryEntity(stationaryEntity)
, m_containerEntity(NULL)
, m_mobileEntity(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

MapNode::~MapNode(void)
{
   if ( m_stationaryEntity )
      delete m_stationaryEntity;

    if ( m_mobileEntity )
        delete m_mobileEntity;

    if ( m_containerEntity )
       delete m_containerEntity;
}

////////////////////////////////////////////////////////////////////////////////

bool MapNode::isEmpty() const
{
   return m_mobileEntity == NULL && m_stationaryEntity == NULL;
}

////////////////////////////////////////////////////////////////////////////////

void MapNode::clear()
{
   delete m_stationaryEntity;
   m_stationaryEntity = NULL;
}

////////////////////////////////////////////////////////////////////////////////

void MapNode::removeEntity(EntityBase* entity)
{
   if (entity)
   {
      if (entity == m_mobileEntity )
         m_mobileEntity = NULL;
      else if (entity == m_containerEntity )
         m_containerEntity = NULL;
      else if (entity == m_stationaryEntity )
         m_stationaryEntity = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool MapNode::isBlocking() const
{
   if (m_stationaryEntity && m_stationaryEntity->isBlocking() )
      return true;

   if (m_mobileEntity && m_mobileEntity->isBlocking() )
      return true;

   return false;
}