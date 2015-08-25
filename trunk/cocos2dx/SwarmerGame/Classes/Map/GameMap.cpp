#include "GameMap.h"
#include <ctime>
#include <cstdlib>
#include "Entities/EntityFactory.h"
#include "Entities/StaticEntity.h"
#include "Entities/MobileEntity.h"
#include "Position.h"
#include "StarNode.h"
#include "MapNode.h"
#include <CCPlatformMacros.h>

GameMap::GameMap(void)
: m_width(0)
, m_height(0)
, m_grid(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

GameMap::~GameMap(void)
{
    destroyMap();
}

////////////////////////////////////////////////////////////////////////////////

void GameMap::generateMap(unsigned int width, unsigned int height)
{
    // check that we have valid parameters to work with
    if ( width && height )
    {
        unsigned int numNodes = width * height;
        // if a map already exists, destroy it
        if ( m_grid )
            destroyMap();

        // allocate the nodes
        if ( (m_grid = new MapNode[numNodes]) )
        {
            m_width = width;
            m_height = height;

//            generateBuildings(params.buildings.maxBuildings, params.buildings.maxWidth, params.buildings.maxHeight);
//            generateWalls(params.walls.maxWalls, params.walls.maxLen);
//            generateTrees(params.trees.maxTrees);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void GameMap::destroyMap()
{
    if ( m_grid )
        delete [] m_grid;
    m_grid = NULL;
    m_width = m_height = 0;
}

////////////////////////////////////////////////////////////////////////////////

MapNode* GameMap::nodeAt(unsigned int x, unsigned int y) const
{
    MapNode* node = NULL;

    if ( x < m_width && y < m_height )
        node = &m_grid[ y * m_width + x ];

    return node;
}

////////////////////////////////////////////////////////////////////////////////

bool GameMap::checkLineOfSight(const Position& fromPos, const Position& toPos, unsigned int lineOfSightLimit) const
{
   int A = toPos.y() - fromPos.y();
   int B = fromPos.x() - toPos.x();
   int C = -(static_cast<int>(A * toPos.x() + B * toPos.y()));

   const int dx = fromPos.x() < toPos.x() ? 1 : fromPos.x() == toPos.x() ? 0 : -1;
   const int dy = fromPos.y() < toPos.y() ? 1 : fromPos.y() == toPos.y() ? 0 : -1;

   Position tmpPos = fromPos;
   while (tmpPos != toPos)
   {
      int hor = std::abs(static_cast<int>(A * (tmpPos.x() + dx) + B * (tmpPos.y() ) + C) );
      int ver = std::abs(static_cast<int>(A * (tmpPos.x()) + B * (tmpPos.y() + dy) + C) );
      int cross = std::abs(static_cast<int>(A * (tmpPos.x() + dx) + B * (tmpPos.y() + dy) + C) );

      if (tmpPos.y() != toPos.y() && (tmpPos.x() == toPos.x() || hor > ver || hor > cross))
         tmpPos.setY(tmpPos.y() + dy);

      if (tmpPos.x() != toPos.x() && (tmpPos.y() == toPos.y() || ver > hor || ver > cross))
         tmpPos.setX(tmpPos.x() + dx);

      MapNode* node = nodeAt( tmpPos.x(), tmpPos.y() );
      if (node == NULL || (node->getStationaryEntity() && node->getStationaryEntity()->isBlocking()) || !(lineOfSightLimit--))
         return false;
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GameMap::isBlocking(const MapNode* node) const
{
   bool bResult = false;
   if (node == NULL || (node->getStationaryEntity() && node->getStationaryEntity()->isBlocking()) )
      bResult = true;
   else if (node->getMobileEntity() )
   {
      if (node->getMobileEntity()->isHuman())
         bResult = true;
   }

   return bResult;
}



////////////////////////////////////////////////////////////////////////////////

template<typename EntityClass>
bool GameMap::placeEntity(EntityClass* entity, int x, int y)
{
   bool bResult = false;
   CC_ASSERT (NULL != entity);
   if (NULL != entity)
   {
      MapNode* nextNode = nodeAt(x, y);
      CC_ASSERT (NULL != nextNode);
      entity->setPos(x, y);
      nextNode->setEntity(entity);
      bResult = true;
   }
   return bResult;
}
template bool GameMap::placeEntity<MobileEntity>(MobileEntity* entity, int x, int y);
template bool GameMap::placeEntity<ContainerEntity>(ContainerEntity* entity, int x, int y);

template<typename EntityClass>
bool GameMap::removeEntity (EntityClass* entity)
{
   bool bResult = false;
   CC_ASSERT (NULL != entity);
   if (NULL != entity)
   {
      MapNode* node = nodeAt(entity->x(), entity->y());
      CC_ASSERT (NULL != node);
      if (NULL != node)
      {
         node->removeEntity(entity);
         bResult = true;
      }
   }
   return bResult;
}
template bool GameMap::removeEntity<MobileEntity>(MobileEntity* entity);
template bool GameMap::removeEntity<ContainerEntity>(ContainerEntity* entity);

////////////////////////////////////////////////////////////////////////////////

template<typename EntityClass>
bool GameMap::moveEntity(EntityClass* entity, int x, int y)
{
   if (entity->x() == x && entity->y() == y)
      return true;

   bool bResult = false;
   CC_ASSERT (NULL != entity);
   if (NULL != entity)
   {
      MapNode* nextNode = nodeAt(x, y);
      CC_ASSERT (NULL != nextNode);
      if (NULL != nextNode && !nextNode->isBlocking())
      {
          MapNode* currNode = nodeAt(entity->x(), entity->y());
          CC_ASSERT (NULL != currNode);
          currNode->removeEntity(entity);
          placeEntity(entity, x, y);
          bResult = true;
      }
   }
   return bResult;
}
template bool GameMap::moveEntity<MobileEntity>(MobileEntity* entity, int x, int y);
template bool GameMap::moveEntity<ContainerEntity>(ContainerEntity* entity, int x, int y);

////////////////////////////////////////////////////////////////////////////////

const StarNode* GameMap::getPartialPath(const Position& startPos, const Position& destPos, StarNodes* nodes, const unsigned int maxNodeCount, int lineOfSightDistance, const PathSearchParams& params)
{
   StarNode* startNode = nodes->getStartNode();

   startNode->x = destPos.x();
   startNode->y = destPos.y();
   startNode->g = 0;
   startNode->h = getEstimatedDistanceCost(destPos.x(), destPos.y(), destPos.x(), destPos.y(), startPos.x(), startPos.y());
   startNode->f = startNode->g + startNode->h;
   startNode->parent = NULL;

   const StarNode* foundNode = NULL;
   Position evalPos;
   MapNode* node;

   unsigned int startNodeIndex = nodes->getCurrentNodeIndex();

   while (nodes->hasOpenNodes() && (nodes->getCurrentNodeIndex() - startNodeIndex) <= maxNodeCount)
   {
      const StarNode* bestNode = nodes->getBestNode();
      if ( NULL == bestNode)
         return NULL; //no path found

      if (bestNode->x == startPos.x() && bestNode->y == startPos.y())
         return bestNode;
      else{
         nodes->closeNode(bestNode);

         int maxSearchDirections = (params.allowDiagonal ? kMaxNeighborCount : 4);
         for (int i = 0; i < maxSearchDirections; ++i)
         {
            int xNeighbour = bestNode->x + sNeighborList[i][0];
            int yNeighbour = bestNode->y + sNeighborList[i][1];

            if (xNeighbour < 0 || yNeighbour < 0)
               continue;

            evalPos.setX(xNeighbour);
            evalPos.setY(yNeighbour);

            bool bHasLineOfSight = !params.keepLineOfSight || checkLineOfSight(evalPos, destPos, lineOfSightDistance);

            node = nodeAt( evalPos.x(), evalPos.y() );
            if (bHasLineOfSight && (evalPos == destPos || evalPos == startPos || !isBlocking(node)) )
            {
               int newg = bestNode->g + getStepCost(bestNode, evalPos);

               //Check if the node is already in the closed/open list
               //If it exists and the nodes already on them has a lower cost (g) then we can ignore this neighbor node
               StarNode* neighbourNode = nodes->getNodeInList(evalPos.x(), evalPos.y());
               if (neighbourNode)
               {
                  //The node on the closed/open list is cheaper than this one
                  if (neighbourNode->g <= newg)
                     continue;

                  nodes->openNode(neighbourNode);
               }
               else
               {
                  //Does not exist in the open/closed list, create a new node
                  neighbourNode = nodes->createOpenNode();

                  //Out of nodes
                  if (NULL == neighbourNode)
                     return NULL;
               }

               //This node is the best node so far with this state
               neighbourNode->x = evalPos.x();
               neighbourNode->y = evalPos.y();
               neighbourNode->parent = bestNode;
               neighbourNode->g = newg;
               neighbourNode->h = getEstimatedDistanceCost(neighbourNode->x, neighbourNode->y, destPos.x(), destPos.y(), startPos.x(), startPos.y());
               neighbourNode->f = neighbourNode->g + neighbourNode->h;
            }
         }
      }
   }

   return NULL;
}

////////////////////////////////////////////////////////////////////////////////

bool GameMap::getPath(const Position& startPos, const Position& destPos, std::list<Position>& path, StarNodes* nodes, unsigned int maxNodeCount, int lineOfSightDistance, const PathSearchParams params)
{
   path.clear();

   const StarNode* foundNode = getPartialPath(startPos, destPos, nodes, maxNodeCount, lineOfSightDistance, params);
   if (foundNode != NULL)
   {
      if (params.popStartPos)
         foundNode = foundNode->parent; //skip startPos

      while (foundNode)
      {
         path.push_back( Position(foundNode->x, foundNode->y) );
         foundNode = foundNode->parent;
      }
   }

   return !path.empty();
}

////////////////////////////////////////////////////////////////////////////////

bool GameMap::getPath(const Position& startPos, const Position& destPos, std::list<Position>& path, int lineOfSightDistance, const PathSearchParams params)
{
   StarNodes nodes;
   StarNode* startNode = nodes.createOpenNode();

   return getPath (startPos, destPos, path, &nodes, StarNodes::kMaxNodes, lineOfSightDistance, params);
}

////////////////////////////////////////////////////////////////////////////////

bool GameMap::getRefugePoint (const Position& enemyPos, const Position& myPos, Position& refugePos)
{
   Position cartEnemy (enemyPos.x(), height () - enemyPos.y ());
   Position cartSelf (myPos.x(), height () - myPos.y ());

   //calculate distance between 2 points
   int distanceX = cartSelf.x() - cartEnemy.x();
   int distanceY = cartSelf.y() - cartEnemy.y();
   int distanceSquare = (distanceX * distanceX) + (distanceY * distanceY);
   
   double distance = sqrt((double)distanceSquare);
   
   //calculate unit vector values for x and y
   double unitX = (distanceX != 0) ? distanceX / distance : 0;
   double unitY = (distanceY != 0) ? distanceY / distance : 0;
   
   //given range for flee
   double magnitude = 3;

   //get outta here...
   unsigned int newX = cartSelf.x() + static_cast<unsigned int>(unitX * magnitude);
   unsigned int newY = height() - (cartSelf.y() + static_cast<unsigned int>(unitY * magnitude));
   
   refugePos = myPos;
   //but stay on map
   if (newX > 0 && newX < width())
      refugePos.setX(newX);
   if (newY > 0 && newY < height())
      refugePos.setY(newY);

   MapNode* node;

   //if refugePos is blocked try again along escape vector
   for (int nRetry = 10; nRetry > 0; --nRetry)
   {
      node = nodeAt( refugePos.x(), refugePos.y() );
      if (isBlocking(node))
      {
         if (refugePos.x() + unitX < width() && refugePos.x() + unitX > 0)
            refugePos.setX(static_cast<unsigned int>(refugePos.x() + unitX));
         if (refugePos.y() + unitY < height() && refugePos.y() + unitY > 0)
            refugePos.setY(static_cast<unsigned int>(refugePos.y() + unitY));
      }
   }   

   //CCLOG ("My position: %d,%d Enemy: %d,%d Refuge: %d,%d\n");
   return true;
}

////////////////////////////////////////////////////////////////////////////////

int GameMap::getStepCost(const StarNode* node, const Position& nextPos) const
{
   bool bDiagonal = std::abs(static_cast<int>(node->x - nextPos.x() )) == std::abs(static_cast<int>(node->y - nextPos.y() ));
   int cost = (bDiagonal ? kStepCostDiagonal : kStepCostNormal);

   MapNode* mapNode = nodeAt(nextPos.x(), nextPos.y());

   //Mobile entities are considered semi-blocking, so we add an extra cost of trying to find a path using such nodes
   if (mapNode->getMobileEntity())
   {
      switch (mapNode->getMobileEntity()->type())
      {
         //ZOMBIES
      case EntityFatZombie:
      case EntityNormalZombie:
      case EntityFastZombie:
      case EntitySuperZombie:
         // HUMANS
      case EntityFatHuman:
      case EntityNormalHuman:
      case EntityFastHuman:
      case EntityPistolHuman:
      case EntityShotgunHuman:
      case EntityMachineGunHuman:
      case EntityMechHuman:
      case EntityTank:
            cost += kStepCostNormal * 5; 
            break;
         default: 
            break;
      }
   }

   //TODO: if we want associate extra costs (ie. a wall that is damaging the zombie)
   //cost += 0

   return cost;
}

////////////////////////////////////////////////////////////////////////////////

int GameMap::getEstimatedDistanceCost(int xStart, int yStart, int xGoal, int yGoal) const
{
   return getEstimatedDistanceCost(xStart, yStart, xStart, yStart, xGoal, yGoal);
}

////////////////////////////////////////////////////////////////////////////////

int GameMap::getEstimatedDistanceCost(int x, int y, int xStart, int yStart, int xGoal, int yGoal) const
{
   /* Reference:
      http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#S7
   */

   int hDiagonal = std::min(std::abs(x - xGoal), std::abs(y - yGoal));
   int hStraight = (std::abs(x - xGoal) + std::abs(y - yGoal));

   int distance = kStepCostDiagonal * hDiagonal + kStepCostNormal * (hStraight - 2 * hDiagonal);
   return distance;

   //tie breaker
   int dx1 = x - xGoal;
   int dy1 = y - yGoal;
   int dx2 = xStart - xGoal;
   int dy2 = yStart - yGoal;
   int cross = std::abs(dx1 * dy2 - dx2 * dy1);

   static const float p = kStepCostNormal / (25 * 1.0f); // The factor p should be chosen so that p <(minimum cost of taking one step)/(expected maximum path length).
   return distance + static_cast<int>(cross * p);
}
