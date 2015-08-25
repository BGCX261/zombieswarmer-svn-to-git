#ifndef _PARKGENERATOR_H_
#define _PARKGENERATOR_H_

#include "MapGeneratorBase.h"
#include "DiamondSquare.h"
#include <CCPlatformMacros.h>
#include <cocos2d.h>
using namespace cocos2d;

class GameMap;

class ParkGenerator : public MapGeneratorBase
{
public:

   struct MapGenParams
   {
      MapGenParams()
      {
         parks.roughness = 1.8f;
         parks.heightThreshold = 0.4f;
         parks.treeChance = 0.05f;
         parks.numberOfRoads = 2;

         parks.seed.middleWeight = 0.0f;
         parks.seed.edge1Weight = 0.0f;
         parks.seed.edge2Weight = 0.0f;
         parks.seed.edge3Weight = 0.0f;
         parks.seed.edge4Weight = 0.0f;

         switch (rand() % 5)
         {
            case 0: parks.seed.middleWeight = 1.0f; break;
            case 1: parks.seed.edge1Weight = 1.0f; break;
            case 2: parks.seed.edge2Weight = 1.0f; break;
            case 3: parks.seed.edge3Weight = 1.0f; break;
            case 4: parks.seed.edge4Weight = 1.0f; break;
         }
      };

      struct
      {
         double roughness;
         double heightThreshold;
         double treeChance;
         int numberOfRoads;

         DiamondSquare::InitialSeed seed;

      } parks;
   };

   explicit ParkGenerator(GameMap* map, MapGenParams params, unsigned int width = 50, unsigned int height = 50);
   ~ParkGenerator(void);

protected:
   void generateMap(const ParkGenerator::MapGenParams& params, unsigned int width, unsigned int height);
   void generateWater(const ParkGenerator::MapGenParams& params, unsigned int width, unsigned int height);

   Position getRandomHorizontalEdgePos();
   Position getRandomVerticalEdgePos();

private:

   GameMap* m_map;
};

#endif//_ParkGenerator_H_