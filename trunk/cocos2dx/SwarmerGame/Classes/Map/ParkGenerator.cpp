#include "ParkGenerator.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/StaticEntity.h"
#include "Map/Position.h"
#include "GameMap.h"
#include "MapNode.h"

namespace 
{
   typedef StaticEntity< EntityWater >    WaterEntity;
   typedef StaticEntity< EntityGrass >    GrassEntity;
   typedef StaticEntity< EntityTree >     TreeEntity;
   typedef StaticEntity< EntityGravel >   GravelEntity;
}

ParkGenerator::ParkGenerator(GameMap* map, ParkGenerator::MapGenParams params, unsigned int width, unsigned int height)
   : m_map(map)
{
   if ( map )
      generateMap(params, width, height);
}

////////////////////////////////////////////////////////////////////////////////

ParkGenerator::~ParkGenerator(void)
{
}

////////////////////////////////////////////////////////////////////////////////

Position ParkGenerator::getRandomVerticalEdgePos()
{
   Position pos;

   switch (rand() % 2)
   {
      case 0: pos.setPos(0, rand() % m_map->width() - 1); break;
      case 1: pos.setPos(m_map->width() - 1, rand() % m_map->height()); break;
   }

   return pos;
}

////////////////////////////////////////////////////////////////////////////////

Position ParkGenerator::getRandomHorizontalEdgePos()
{
   Position pos;

   switch (rand() % 2)
   {
      case 0: pos.setPos(rand() % m_map->width() - 1, 0); break;
      case 1: pos.setPos(rand() % m_map->width(), m_map->height() - 1); break;
   }

   return pos;
}

////////////////////////////////////////////////////////////////////////////////

void ParkGenerator::generateMap(const ParkGenerator::MapGenParams& params, unsigned int width, unsigned int height)
{
   // create the MapNodes in the map for the required width and height
   m_map->generateMap(width, height);

   int sinWavePosition = 0;
   int cosWavePosition = 0;
   int waveMultiplier= 10;
   int waveStretcher = 5;

   generateWater(params, width, height);

   for ( unsigned int y = 0; y < m_map->height(); y++ )
   {
      for ( unsigned int x = 0; x < m_map->width(); x++ )
      {
         if (isRegionEmpty(*m_map, x, y, 1, 1) )
         {
            if (params.parks.treeChance * 100 >= rand() % 100 )
               placeEntity< TreeEntity>(*m_map, x, y, 1, 1);
            else
               placeEntity< GrassEntity>(*m_map, x, y, 1, 1);
         }
      }
   }
   
   int builtRoads = 0;
   while (builtRoads < params.parks.numberOfRoads)
   {
      Position startPos;
      Position dstPos;

      if (rand() % 2 == 0)
      {
         //horizontal axis
         startPos = getRandomHorizontalEdgePos();
         dstPos = getRandomVerticalEdgePos();
      }
      else
      {
         //vertical axis
         startPos = getRandomVerticalEdgePos();
         dstPos = getRandomHorizontalEdgePos();
      }

      GameMap::PathSearchParams pathParams;
      pathParams.allowDiagonal = false;
      pathParams.popStartPos = false;
      pathParams.keepLineOfSight = false;

      std::list<Position> path;
      if (m_map->getPath(startPos, dstPos, path, kLimitOfLineOfSight, pathParams))
      {
         ++builtRoads;
         for(std::list<Position>::const_iterator iter = path.begin(); iter != path.end(); ++iter)
         {
            placeEntity< GravelEntity>(*m_map, iter->x(), iter->y(), 1, 1, true);
         }
      }
   }
}

void ParkGenerator::generateWater(const ParkGenerator::MapGenParams& params, unsigned int width, unsigned int height)
{
   DiamondSquare ds(width, height, params.parks.roughness);
   WeightArray points = ds.generate(params.parks.seed);

   for ( unsigned int y = 0; y < height; ++y )
   {
      for ( unsigned int x = 0; x < width; ++x )
      {
         double height = points[x][y];
         //CCLOG("noise level: %f " , height);

         if(height >= params.parks.heightThreshold)
            placeEntity< WaterEntity>(*m_map, x, y, 1, 1);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

