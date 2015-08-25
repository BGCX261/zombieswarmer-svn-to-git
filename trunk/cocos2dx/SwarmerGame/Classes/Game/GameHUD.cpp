#include <sstream>
#include "GameState.h"
#include "GameHUD.h"

using namespace cocos2d;

GameHUD::GameHUD(void)
: m_lblZombieCount(NULL)
, m_lblHumanCount(NULL)
, m_lblTime(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

GameHUD::~GameHUD(void)
{
   m_lblZombieCount->removeFromParentAndCleanup(true);
   m_lblHumanCount->removeFromParentAndCleanup(true);
   m_lblTime->removeFromParentAndCleanup(true);
}

////////////////////////////////////////////////////////////////////////////////

void GameHUD::initWithGame(GameState* game)
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();

   CCLayer::init();

   m_game = game;

   CCLayerColor* panel = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 128), winSize.width*0.95f, 30 );
   panel->setAnchorPoint( ccp(0, 0) );
   panel->setPosition( ccp( winSize.width/2 - panel->getContentSize().width/2, winSize.height - panel->getContentSize().height - 5 ) );

   m_lblZombieCount = CCLabelTTF::labelWithString("", "Arial", 20);
   m_lblZombieCount->setAnchorPoint( ccp(0, 0) );

   m_lblHumanCount = CCLabelTTF::labelWithString("", "Arial", 20);
   m_lblHumanCount->setAnchorPoint( ccp(0, 0) );

   m_lblTime = CCLabelTTF::labelWithString("00:00", "Arial", 20);
   m_lblTime->setAnchorPoint( ccp(0, 0) );

   refreshFromState(0.0f);

   m_lblZombieCount->setPosition( ccp( 10, 0 ) );
   m_lblHumanCount->setPosition( ccp( panel->getContentSize().width - m_lblHumanCount->getContentSize().width - 10, 0 ) );
   m_lblTime->setPosition( ccp( panel->getContentSize().width / 2 - m_lblTime->getContentSize().width/2, 0 ) );

   panel->addChild(m_lblZombieCount);
   panel->addChild(m_lblHumanCount);
   panel->addChild(m_lblTime);

   addChild(panel);
}

////////////////////////////////////////////////////////////////////////////////

void GameHUD::refreshFromState(ccTime dt)
{
   if ( m_game )
   {
      static float timeCumulative = 0.0f;
      std::stringstream ss;

      ss << "Zombies: " << m_game->zombieCount();
      m_lblZombieCount->setString(ss.str().c_str());
      ss.str("");

      ss << "Humans: " << m_game->humanCount();
      m_lblHumanCount->setString(ss.str().c_str());
      ss.str("");

      timeCumulative += dt;
      if ( timeCumulative >= 1.0f )
      {
         ss.width(2);
         ss.fill('0');
         ss << (int)timeCumulative / 60 << ":";
         ss.width(2);
         ss.fill('0');
         ss << (int)timeCumulative % 60;
         m_lblTime->setString(ss.str().c_str());
      }
   }
}