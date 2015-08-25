#ifndef _CONTAINER_ENTITY_H_
#define _CONTAINER_ENTITY_H_

#include "EntityBase.h"
#include <queue>

class ContainerEntity;

typedef std::vector<EntityBase*> ItemList;

class ContainerEntity : public EntityBase
{
public:
   class Iterator
   {
   public:
      Iterator();
      Iterator(const Iterator& rhs)
         : m_container(rhs.m_container)
         , m_current(rhs.m_current)
         {
            /*NO IMPL*/
         }

      Iterator& operator=(const Iterator& rhs)
      {
         m_container = rhs.m_container;
         m_current = rhs.m_current;
         return *this;
      }

      bool operator==(const Iterator& rhs)
      {
         if(m_container != rhs.m_container)
            return true;

         return !(*this != rhs);
      }
      bool operator!=(const Iterator& rhs) { return m_current != rhs.m_current; }

      EntityBase* operator*() { return *m_current; }
      EntityBase* operator->() { return *(*this); }

      Iterator& operator++()
      {
         ++m_current;
         return *this;
      }

      Iterator operator++(int)
      {
         Iterator tmp(*this);
         ++*this;
         return tmp;
      }

   private:
	   Iterator(ContainerEntity* container) : m_container(container) { /*NO IMPL*/}

      ContainerEntity* m_container;
      ItemList::iterator m_current;
      friend class ContainerEntity;
   };

   ContainerEntity(unsigned long id, EntityType type) :
      EntityBase(id, type, false)
      {}

   virtual void step(float timeSlice) { /*NO IMPL*/ }

   size_t size() const {return items.size();}
   bool empty() const {return items.empty();}

   Iterator begin()
   {
      Iterator cit(this);
      if (!items.empty() )
      {
         cit.m_current = items.begin();
      }
   }

   Iterator end() { return Iterator(this); }
   Iterator begin() const { return const_cast<ContainerEntity*>(this)->begin(); }
   Iterator end() const { return const_cast<ContainerEntity*>(this)->end(); }

private:
   ItemList items;
};

#endif//_CONTAINER_ENTITY_H_