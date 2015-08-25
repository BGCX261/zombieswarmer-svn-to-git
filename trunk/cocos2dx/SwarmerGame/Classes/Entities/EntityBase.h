#ifndef _ENTITY_BASE_H_
#define _ENTITY_BASE_H_

#include <vector>
#include "Entities.h"

class EntitySwarm;


class EntityBase 
{
public:
   EntityBase(unsigned long id, EntityType type, bool isMoveable = false);
   virtual ~EntityBase(void);

   virtual void step(float timeSlice) = 0;
   virtual bool isHuman () { return false; };

   unsigned long id() const                    { return m_id; }
   EntityType type() const                     { return m_type; }
   void setPos(unsigned int x, unsigned int y);
   unsigned int x() const                      { return m_x; }
   unsigned int y() const                      { return m_y; }

   bool hasLastPosition() const                { return !(m_lastX == 0xFFFFFFFF || m_lastY == 0xFFFFFFFF); }
   unsigned int lastX() const                  { return m_lastX; }
   unsigned int lastY() const                  { return m_lastY; }
   void setLastPosition(unsigned int x, unsigned int y)           { m_lastX = x; m_lastY = y; }


   bool isMoveable() const                     { return m_isMoveable; }
   bool isBlocking() const;


   void setSwarm(EntitySwarm* swarm) { m_swarm = swarm; }
   EntitySwarm* swarm() const { return m_swarm; }  

protected:
   // entity can belong to the swarm
   EntitySwarm*    m_swarm;

protected:

   unsigned long   m_id;

   EntityType      m_type;


   // entity position within the world map
   unsigned int    m_x, m_y;
   unsigned int    m_lastX, m_lastY;   

   // flag indicator whether this entity is moveable
   bool            m_isMoveable;

};

#endif//_ENTITY_BASE_H_