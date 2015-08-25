#ifndef _CITYGENERATOR_H_
#define _CITYGENERATOR_H_

#include "MapGeneratorBase.h"

class GameMap;

class CityGenerator : public MapGeneratorBase
{
public:

   struct MapGenParams
   {
      MapGenParams()
      {
         roads.width = 1;
         roads.turnTendency = 10;
         buffs.maxAmmoSpots = 5;
      };

      struct Buffs {
         int maxAmmoSpots;
      } buffs;

      struct
      {
         int width;
         int turnTendency;
      } roads;
   };

   explicit CityGenerator(GameMap* map, MapGenParams params, unsigned int width = 50, unsigned int height = 50);
   ~CityGenerator(void);

protected:

   void generateMap(GameMap* map, CityGenerator::MapGenParams params, unsigned int width = 50, unsigned int height = 50);

   void generateMapBorder(GameMap* map);
   void generateRoads(GameMap* map, int roadWidth = 1, int turnTendency = 10);
   void generateLots(GameMap* map);
   void generateBuffs(GameMap* map, MapGenParams::Buffs params);
   void generateWalls(GameMap* map, unsigned int maxWalls = 10, unsigned int maxLen = 5);

private:

   void updateStepsForTurn(int& xStep, int& yStep);
};

#endif//_CITYGENERATOR_H_