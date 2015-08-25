#include "EntityBase.h"
#include <climits>

EntityBase::EntityBase(unsigned long id, EntityType type, bool isMoveable)
: m_id(id)
, m_type(type)
, m_x(UINT_MAX)
, m_y(UINT_MAX)
, m_lastX(UINT_MAX)
, m_lastY(UINT_MAX)
, m_isMoveable(isMoveable)
, m_swarm(NULL)
{
}

EntityBase::~EntityBase(void)
{
}

void EntityBase::setPos(unsigned int x, unsigned int y)
{
   m_lastX = m_x;
   m_lastY = m_y;
   m_x = x;
   m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

bool EntityBase::isBlocking() const
{
   bool bResult = false;
   switch (type() )
   {
   case  EntityNone:
   case  EntityRoad:
   case  EntityGrass:
      break;

   default:
      bResult = true;
      break;
   }

   return bResult;
}

