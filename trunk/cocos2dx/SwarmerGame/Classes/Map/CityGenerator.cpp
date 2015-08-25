#include "CityGenerator.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/StaticEntity.h"
#include "GameMap.h"
#include "MapNode.h"

namespace 
{
   typedef StaticEntity< EntityBuilding >    BuildingEntity;
   typedef StaticEntity< EntityWall >        WallEntity;
   typedef StaticEntity< EntityTree >        TreeEntity;
   typedef StaticEntity< EntityRoad >        RoadEntity;
   typedef StaticEntity< EntityAmmo >        AmmoEntity;
}

CityGenerator::CityGenerator(GameMap* map, CityGenerator::MapGenParams params, unsigned int width, unsigned int height)
{
   if ( map )
      generateMap(map, params, width, height);
}

////////////////////////////////////////////////////////////////////////////////

CityGenerator::~CityGenerator(void)
{
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::generateMap(GameMap* map, CityGenerator::MapGenParams params, unsigned int width, unsigned int height)
{
   // create the MapNodes in the map for the required width and height
   map->generateMap(width, height);

   generateMapBorder(map);

   generateRoads(map, params.roads.width, params.roads.turnTendency);
   generateLots(map);
   generateBuffs(map, params.buffs);
//   generateWalls(map, params.walls.maxWalls, params.walls.maxLen);
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::generateMapBorder(GameMap* map)
{
   int w = map->width(), h = map->height();

   for ( int i = 0; i < w; i++ )
   {
      placeEntity< WallEntity >(*map, i, 0, 1, 1);
      placeEntity< WallEntity >(*map, i, h-1, 1, 1);
   }
   for ( int i = 0; i < h; i++ )
   {
      placeEntity< WallEntity >(*map, 0, i, 1, 1);
      placeEntity< WallEntity >(*map, w-1, i, 1, 1);
   }
}

////////////////////////////////////////////////////////////////////////////////
//
// Need to document this function when it works correctly... its a mess!
void CityGenerator::generateRoads(GameMap* map, int roadWidth, int turnTendency)
{
   const int minLotWidth = 3, minLotHeight = 3;

   // generate the horizontal roads (these will always run from end to end)
   for ( unsigned int y = 1; y < map->height(); y += minLotHeight )
   {
      y += rand() % minLotHeight;
      if ( y >= map->height() )
         break;

      for ( int w = 0; w < roadWidth; w++ )
      {
         y++;
         if ( y < map->height() )
         {
            for ( unsigned int x = 1; x < map->width(); x++ )
            {
               placeEntity< RoadEntity >(*map, x, y, 1, 1);
            }
         }
      }
   }

   // now generate the vertical roads. These have a chance of terminating when intersecting with a horizontal (based on turnTendency).
   // This termination gives rise to larger lots, and provide a break to a monotonous grid layout...
   for ( unsigned int x = 1; x < map->width(); x += minLotWidth )
   {
      x += rand() % minLotWidth;
      if ( x >= map->width() )
         break;

      for ( int w = 0; w < roadWidth; w++ )
      {
         x ++;
         if ( x < map->width() )
         {
            for ( unsigned int y = 0; y < map->height(); y++ )
            {
               if ( !isRegionEmpty(*map, x, y, 1, 1) )
               {
                  // check if we should skip the next segment of road
                  if ( (rand() % 100) < turnTendency )
                  {
                     // find the next segment
                     y++;
                     while ( isRegionEmpty(*map, x, y, 1, 1 ) )
                     {
                        y++;
                        if ( y >= map->height() )
                           break;
                     }
                  }
               }
               else
               {
                  placeEntity< RoadEntity >(*map, x, y, 1, 1);
               }
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::generateLots(GameMap* map)
{
   MapNode* node = NULL;

   for ( unsigned int y = 1; y < map->height(); y++ )
   {
      for ( unsigned int x = 1; x < map->width(); x++ )
      {
         if ( (node = map->nodeAt(x, y)) )
         {
            if ( node->isEmpty() )
            {
               // buildings have a 2/3 chance of being placed... trees only 1/3 chance
               switch ( rand() % 3 )
               {
               case  0:
               case  1:
                  placeEntity< BuildingEntity >(*map, x, y, 1, 1);
                  break;

               case  2:
                  placeEntity< TreeEntity >(*map, x, y, 1, 1);
                  break;
               }
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::generateBuffs(GameMap* map, MapGenParams::Buffs params)
{
   if (params.maxAmmoSpots > 0)
   {
      MapNode* node = NULL;
      bool bStop = false;
      for ( unsigned int y = 1; y < map->height() && !bStop; y++ )
      {
         for ( unsigned int x = 1; x < map->width() && !bStop; x++ )
         {
            if ( (node = map->nodeAt(x, y)) )
            {
               if ( !node->isBlocking() )
               {
                  if (rand() % 100 < 10)
                  {
                     placeEntity< AmmoEntity >(*map, x, y, 1, 1);

                     if (--params.maxAmmoSpots == 0)
                        bStop = true;
                  }
               }
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::generateWalls(GameMap* map, unsigned int maxWalls, unsigned int maxLen)
{
    EntityFactory& factory = EntityFactory::instance();
    unsigned int numWalls = 1 + ( rand() % maxWalls );
    MapNode* node = NULL;
    EntityBase* entity = NULL;

    for ( unsigned int i = 0; i < numWalls; i++ )
    {
        int direction = rand() % 4;         // 0 = up, 1 = right, 2 = down, 3 = left
        int x, y, width, height;
        int xStep = direction == 1 ? 1 : direction == 3 ? -1 : 0;
        int yStep = direction == 0 ? -1 : direction == 2 ? 1 : 0;
        int wallLen = 1 + rand() % maxLen;

        // get a random node (to start the wall)
        x = rand() % map->width();
        y = rand() % map->height();

        switch ( direction )
        {
        case    0:
            y -= wallLen;
            width = 1;
            height = wallLen;
            break;

        case    1:
            width = wallLen;
            height = 1;
            break;

        case    2:
            width = 1;
            height = wallLen;
            break;

        case    3:
            x -= wallLen;
            width = wallLen;
            height = 1;
            break;
        }
        
        if ( isRegionEmpty(*map, x, y, width, height) )
        {
            if ( !placeEntity< WallEntity >(*map, x, y, width, height) )
                i--;
        }
        else
        {
            i--;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void CityGenerator::updateStepsForTurn(int& xStep, int& yStep)
{
   /*
      xStep == 1 means moving RIGHT
      xStep == -1 means moving LEFT
      yStep == 1 means moving DOWN
      yStep == -1 means moving UP
   */

   if ( xStep )
   {
      xStep = 0;
      while ( !yStep )
         yStep = -1 + (rand() % 2);
   }
   else if ( yStep )
   {
      yStep = 0;
      while ( !xStep )
         xStep = -1 + (rand() % 2);
   }
}