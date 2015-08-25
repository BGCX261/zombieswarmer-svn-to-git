/***
 *
 *  This class manages the collection entities within a swarm and responsible for visiting each particle per update
 *  and allowing the particles respective behaviour functions to execute
 *
 */
#ifndef _ENTITYSWARM_H_
#define _ENTITYSWARM_H_

#include <map>
#include <vector>
#include "EntityBase.h"
#include "../Map/Position.h"

class GameMap;
class MobileEntity;

class EntitySwarm
{
    typedef std::map< unsigned long, MobileEntity* >          EntityMap;
    typedef std::vector< EntityBase* >                      EntityVector;

public:
    EntitySwarm(void);
    ~EntitySwarm(void);

    void resetSwarmHistory ();

    void setMap(GameMap* map) { m_map = map; }
    GameMap* gameMap() { return m_map; }

    void addEntity(MobileEntity* entity);
    void removeEntity(MobileEntity* entity, bool cleanup = false);
    size_t size() { return m_entities.size(); }

    void addTarget(EntityBase* target);
    void setTargets(const std::vector< EntityBase* >& targetList);
    void removeTarget(EntityBase* target);
    void setTargetOfGod(EntityBase* targetOfGod);

    std::vector<EntityBase*>& targets() { return m_targets; }
    const std::vector<EntityBase*>& targets() const { return m_targets; }

    void step(float timeSlice);

    Position centerPoint();

    unsigned int entitiesInRect(Rect<unsigned int> rect, std::vector< EntityBase* >& targets) const;

    void dumpEntities();

    EntityBase* targetOfGod() { return m_targetOfGod; }

private:

    bool targetIsValid(EntityBase* target);

private:

    EntityMap                       m_entities;
    EntityVector                    m_targets;
    EntityBase*                     m_targetOfGod;
    GameMap*                        m_map;
    Rect<int>                       m_boundingRect;
};

#endif//_ENTITYSWARM_H_