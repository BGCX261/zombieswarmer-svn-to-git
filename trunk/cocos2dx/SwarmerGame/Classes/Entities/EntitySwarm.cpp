#include "cocos2d.h"
#include "EntitySwarm.h"
#include "MobileEntity.h"

EntitySwarm::EntitySwarm(void) 
   : m_map (NULL)
   , m_targetOfGod (NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

EntitySwarm::~EntitySwarm(void)
{
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::addEntity(MobileEntity* entity)
{
    if ( entity )
    {
        EntityMap::const_iterator it = m_entities.find(entity->id());

        if ( it == m_entities.end() )
        {
            if (entity->swarm() != this)
               removeEntity(entity, false);
            entity->setSwarm(this);
            m_entities[ entity->id() ] = entity;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::removeEntity(MobileEntity* entity, bool cleanup)
{
    if ( entity )
    {
        EntityMap::const_iterator it = m_entities.find( entity->id() );

        if ( it != m_entities.end() )
        {
            if (it->second)  it->second->setSwarm(NULL);
            // pop the entity out of th map
            m_entities.erase(it);
            // if we should cleanup as well, do it now
            if ( cleanup )
            {
                delete entity;
                entity = NULL;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::addTarget(EntityBase* target)
{
    if (NULL != target)
    {
       m_targets.push_back(target);
    }
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::setTargets(const std::vector< EntityBase* >& targetList)
{
   m_targets.clear();
   std::vector< EntityBase* >::const_iterator it(targetList.begin()), end(targetList.end());
   for ( ; it != end; ++it)
      addTarget(*it);
}


////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::removeTarget(EntityBase* target)
{
    EntityVector::iterator it = m_targets.begin(), limit = m_targets.end();

    for ( ; it != limit; ++it )
    {
        if ( *it == target )
        {
            m_targets.erase(it);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::step(float timeSlice)
{
    EntityMap::const_iterator it = m_entities.begin(), limit = m_entities.end();

    resetSwarmHistory ();

    unsigned int left = 99999, top = 99999, right = 0, bottom = 0;

    for ( ; it != limit; ++it )
    {
       MobileEntity* mob = it->second;
       if (NULL != mob)
         mob->updateMobileProperties(timeSlice);
       it->second->step(timeSlice);

       if ( it->second->x() < left )     left = it->second->x();
       if ( it->second->x() > right )    right = it->second->x();
       if ( it->second->y() < top )      top = it->second->y();
       if ( it->second->y() > bottom )   bottom = it->second->y();
    }

    m_boundingRect = Rect<int>(left, top, right-left, bottom-top);
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::resetSwarmHistory ()
{
    EntityMap::const_iterator it = m_entities.begin(), limit = m_entities.end();
    for ( ; it != limit; ++it )
    {
       MobileEntity* entity = it->second;
       if (NULL != NULL)
          entity->mobileStats().reset();
    }
}

////////////////////////////////////////////////////////////////////////////////

Position EntitySwarm::centerPoint()
{
   return m_boundingRect.center();
}

////////////////////////////////////////////////////////////////////////////////

unsigned int EntitySwarm::entitiesInRect(Rect<unsigned int> rect, std::vector< EntityBase* >& targets) const
{
   unsigned int ret = 0;

   if ( !rect.isEmpty() )
   {
      EntityMap::const_iterator it = m_entities.begin(), limit = m_entities.end();
      unsigned int x, y;

      for ( ; it != limit; ++it )
      {
         x = it->second->x();
         y = it->second->y();

         if ( rect.pointInRect( Position(x, y) ) )
            targets.push_back(it->second);
      }
   }

   return targets.size();
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::dumpEntities()
{
   EntityMap::iterator it = m_entities.begin(), limit = m_entities.end();

   for ( ; it != limit; ++it )
   {
      CCLOG("Entity[%d] at %d, %d", it->second->id(), it->second->x(), it->second->y());
   }
}

////////////////////////////////////////////////////////////////////////////////

void EntitySwarm::setTargetOfGod( EntityBase* targetOfGod )
{
   m_targetOfGod = targetOfGod;
}
