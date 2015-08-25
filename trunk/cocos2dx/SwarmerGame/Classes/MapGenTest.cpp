#include "MapGenTest.h"
#include "Entities/Entities.h"
#include "Entities/EntityFactory.h"
#include "Entities/Zombies/ZombieBase.h"
#include "Map/CityGenerator.h"
#include "Map/ParkGenerator.h"

using namespace cocos2d;

const int c_blockSize = 32;

MapGenTest::MapGenTest(void)
: m_mapLayer(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

MapGenTest::~MapGenTest(void)
{
}

////////////////////////////////////////////////////////////////////////////////

cocos2d::CCScene* MapGenTest::scene()
{
    CCScene * scene = NULL;

    do      /* enclode the body in a 'break-able' closure */
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MapGenTest* layer = MapGenTest::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

////////////////////////////////////////////////////////////////////////////////

bool MapGenTest::init()
{
   bool bRet = false;

   do      /* enclose the body in a 'break-able' closure */
   {
      CC_BREAK_IF( !CCLayer::init() );

      setIsTouchEnabled(true);

/*
      CityGenerator::MapGenParams params;
      CityGenerator city(&m_map, params, 50, 50);
*/

      ParkGenerator::MapGenParams params;
      ParkGenerator city(&m_map, params, 50, 50);

      m_mapLayer = new MapLayer(&m_map, c_blockSize);
      m_mapLayer->init();
      m_mapLayer->setAnchorPoint( ccp(0, 0) );
      m_mapLayer->setPosition( ccp(0, 0) );
      m_mapLayer->autorelease();
      addChild(m_mapLayer);

      bRet = true;
   } while (0);

   return bRet;
}

////////////////////////////////////////////////////////////////////////////////

void MapGenTest::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

////////////////////////////////////////////////////////////////////////////////

bool MapGenTest::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
   m_lastTouchPoint = touch->locationInView( touch->view() );
   return true;
}

////////////////////////////////////////////////////////////////////////////////

void MapGenTest::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   CCPoint pt = touch->locationInView( touch->view() );
   static const float panEpsilon = 3.0;

   // only enable panning if we're not showing the full map
   float panDeltaX = m_lastTouchPoint.x - pt.x;
   float panDeltaY = m_lastTouchPoint.y - pt.y;

   if ( abs(panDeltaX) >= panEpsilon || abs(panDeltaY) >= panEpsilon )
   {
      CCSize mapSize = m_mapLayer->getContentSize();
      CCPoint mapPos = m_mapLayer->getPosition();

      mapPos.x -= panDeltaX;
      mapPos.y += panDeltaY;

      if ( mapPos.y > 0 )     mapPos.y = 0;
      if ( mapPos.x > 0 )     mapPos.x = 0;
      if ( mapPos.y + mapSize.height < winSize.height )        
         mapPos.y += (winSize.height - (mapPos.y + mapSize.height));
      if ( mapPos.x + mapSize.width < winSize.width )
         mapPos.x += (winSize.width - (mapPos.x + mapSize.width));

      m_mapLayer->setPosition(mapPos);
   }

   m_lastTouchPoint = pt;
}

////////////////////////////////////////////////////////////////////////////////

void MapGenTest::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
   // put target creation/update here
   m_lastTouchPoint = ccp(-1, -1);
}
