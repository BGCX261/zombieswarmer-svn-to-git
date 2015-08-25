#ifndef _MAPGENERATORBASE_H_
#define _MAPGENERATORBASE_H_

#include "GameMap.h"
#include "MapNode.h"

class MapGeneratorBase
{
public:
   MapGeneratorBase(void) {}
   virtual ~MapGeneratorBase(void) {}

protected:

   bool isRegionEmpty(GameMap& map, int x, int y, int width, int height)
   {
      bool bResult = true;
      MapNode* node = NULL;

      for ( int row = 0; row < height && bResult; row++ )
      {
         for ( int col = 0; col < width; col++ )
         {
               if ( (node = map.nodeAt(x + col, y + row)) )
               {
                  if ( !node->isEmpty() )
                  {
                     bResult = false;
                     break;
                  }
               }
               else
               {
                  bResult = false;
                  break;
               }
         }
      }

      return bResult;
   }

   ////////////////////////////////////////////////////////////////////////////////

   template <class T>
   bool placeEntity(GameMap& map, int x, int y, int width, int height, bool overwrite = false)
   {
      bool bResult = true;
      MapNode* node = NULL;
      EntityFactory& factory = EntityFactory::instance();
      T* entity = NULL;

      for ( int row = 0; row < height && bResult; row++ )
      {
         for ( int col = 0; col < width; col++ )
         {
            node = map.nodeAt(x + col, y + row);
            if (overwrite)
               node->clear();

            if ( (entity = dynamic_cast<T*>(factory.createEntity<T>())) )
            {
               if (!node->setEntity<T>( entity ))
               {
                  delete entity;
                  bResult = false;
                  break;
               }
            }
            else
            {
               bResult = false;
               break;
            }
         }
      }

      return bResult;
   }

};

#endif//_MAPGENERATORBASE_H_