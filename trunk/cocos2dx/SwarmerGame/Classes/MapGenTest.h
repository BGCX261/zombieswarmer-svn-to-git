#ifndef _MAPGENTEST_H_
#define _MAPGENTEST_H_

#include "cocos2d.h"
#include <vector>
#include "Map/GameMap.h"
#include "Game/MapLayer.h"

using namespace cocos2d;

class GameMap;

class MapGenTest : public cocos2d::CCLayer
{
public:
   MapGenTest(void);
   ~MapGenTest(void);

   static cocos2d::CCScene* scene();

   // called from the 'LAYER_NODE_FUNC()' mechanism defined below
   virtual bool init();  

   void registerWithTouchDispatcher();
   bool ccTouchBegan(CCTouch* touch, CCEvent* event);
   void ccTouchMoved(CCTouch* touch, CCEvent* event);
   void ccTouchEnded(CCTouch* touch, CCEvent* event);

   // implement the "static node()" method manually
   LAYER_NODE_FUNC(MapGenTest);

private:

   GameMap                 m_map;
   MapLayer*               m_mapLayer;
   CCPoint                 m_lastTouchPoint;
};

#endif//_MAPGENTEST_H_