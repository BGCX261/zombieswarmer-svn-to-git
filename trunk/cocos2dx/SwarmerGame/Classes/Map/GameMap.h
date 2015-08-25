#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

#include <list>

class EntityBase;
class Position;
class StarNode;
class StarNodes;
class MapNode;

/**
 *
 *  A GameMap object represents the playing area / map of the game.
 *  The area is a rectangular area composed of a collection of squares, each square representing a node that can be occupied by an entity.
 */
class GameMap
{
public:

    GameMap(void);
    ~GameMap(void);

    void generateMap(unsigned int width, unsigned int height);
    void destroyMap();

    MapNode* nodeAt(unsigned int x, unsigned int y) const;

    unsigned int width() const                      { return m_width; }
    unsigned int height() const                     { return m_height; }

    struct PathSearchParams
    {
       PathSearchParams()
       {
          allowDiagonal = true;
          popStartPos = true;
          keepLineOfSight = true;
       }

       bool allowDiagonal;
       bool popStartPos;
       bool keepLineOfSight;
    };

    bool getPath(const Position& startPos, const Position& destPos, std::list<Position>& path, StarNodes* nodes, unsigned int maxNodeCount, int lineOfSightDistance, const PathSearchParams params = PathSearchParams() );
    bool getPath(const Position& startPos, const Position& destPos, std::list<Position>& path, int lineOfSightDistance, const PathSearchParams params = PathSearchParams() );

    //bool getFleePath(const Position& startPos, const Position& destPos, std::list<Position>& path, StarNodes* nodes, int maxNodeCount);
    bool getRefugePoint (const Position& startPos, const Position& destPos, Position& refugePos);

    bool checkLineOfSight(const Position& fromPos, const Position& toPos, unsigned int lineOfSightLimit) const;

    template<typename EntityClass>
    bool removeEntity (EntityClass* entity);

    template<typename EntityClass>
    bool moveEntity(EntityClass* entity, int x, int y);

    template<typename EntityClass>
    bool placeEntity(EntityClass* entity, int x, int y);
    
    int getEstimatedDistanceCost(int xStart, int yStart, int xGoal, int yGoal) const;

    int getProximityDistanceThreshold() const {return kStepCostNormal * 12;}

private:
   bool isBlocking(const MapNode* node) const;
   const StarNode* getPartialPath(const Position& startPos, const Position& destPos, StarNodes* nodes, const unsigned int maxNodeCount, int lineOfSightDistance, const PathSearchParams& params);

   unsigned int        m_width;
   unsigned int        m_height;
   MapNode*            m_grid;

    static const unsigned int kStepCostNormal   = 1000;
    static const unsigned int kStepCostDiagonal = 2000;

    int getStepCost(const StarNode* node, const Position& nextPos) const;
    int getEstimatedDistanceCost(int x, int y, int xStart, int yStart, int xGoal, int yGoal) const;
};

#endif//_PLAYGROUND_H_