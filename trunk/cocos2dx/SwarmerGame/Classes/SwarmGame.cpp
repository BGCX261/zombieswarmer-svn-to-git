#include <sstream>
#include "SwarmGame.h"
#include "Entities/EntityFactory.h"
#include "Entities/Zombies/ZombieBase.h"
#include "Entities/Humans/HumanBase.h"
#include "Entities/xmlDataLoader.h"
#include "Map/CityGenerator.h"
#include <algorithm>



using namespace cocos2d;

const int c_blockSize = 64;

SwarmGame::SwarmGame(void)
: m_mapLayer(NULL)
, m_showFullMap(false)
{
}

////////////////////////////////////////////////////////////////////////////////

SwarmGame::~SwarmGame(void)
{
}

////////////////////////////////////////////////////////////////////////////////

cocos2d::CCScene* SwarmGame::scene()
{
    CCScene * scene = NULL;

    do      /* enclose the body in a 'break-able' closure */
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        SwarmGame* layer = SwarmGame::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

////////////////////////////////////////////////////////////////////////////////

bool SwarmGame::init()
{
   bool bRet = false;

   do      /* enclose the body in a 'break-able' closure */
   {
      CC_BREAK_IF( !CCLayer::init() );

      CCSize winSize = CCDirector::sharedDirector()->getWinSize();

      setIsTouchEnabled(true);

      // comment this out if you want to be able to pan around the map instead
      //m_showFullMap = true;

      // generate the map data
      CityGenerator::MapGenParams params;
      params.roads.width = 3;
      CityGenerator city(&m_map, params, 50, 50);

      // generate the map layer (visualisation)
      m_mapLayer = new MapLayer(&m_map, c_blockSize);
      m_mapLayer->init();
      m_mapLayer->setAnchorPoint( ccp(0, 0) );
      m_mapLayer->setPosition( ccp(0, 0) );
      m_mapLayer->autorelease();

      if ( m_showFullMap )
      {
         m_mapLayer->setAnchorPoint( ccp(0.0, 0.0) );
         m_mapLayer->setScaleX( winSize.width / m_mapLayer->getContentSize().width );
         m_mapLayer->setScaleY( winSize.height / m_mapLayer->getContentSize().height );
      }
      addChild(m_mapLayer);

      // create the visibility field overlay (comment out if you need to see the whole view)
      createVisibilityField();

      m_HumanSwarm.setMap(&m_map);
      m_ZombieSwarm.setMap(&m_map);

      initialiseSwarm (EntityFatZombie, 5);
      initialiseSwarm (EntityNormalZombie, 1);
      initialiseSwarm (EntityFastZombie, 1);
      initialiseSwarm (EntitySuperZombie, 1);
      
      initialiseSwarm (EntityNormalHuman, 10);
      initialiseSwarm (EntityFatHuman, 10);
      initialiseSwarm (EntityFastHuman, 2);
      initialiseSwarm (EntityPistolHuman, 1);
      initialiseSwarm (EntityShotgunHuman, 1);
      initialiseSwarm (EntityMechHuman, 1);
      initialiseSwarm (EntityTank, 5);

      // initialise the click indicator particle system
   	ccColor4F startColor = {0.4f, 0.0f, 0.0f, 1.0f};
   	ccColor4F endColor = {0.1f, 0.0f, 0.0f, 0.2f};
      ccColor4F varColor = {0.0f, 0.0f, 0.0f};

      m_clickMarker = CCParticleSun::node();
      // position the marker offscreen so that it doesn't leave a remnant when it becomes visible the first time
      m_clickMarker->setPosition( ccp(-100, -100) );     
      m_clickMarker->setIsVisible(false);
      m_clickMarker->setTexture( CCTextureCache::sharedTextureCache()->addImage("comet.png") );
      m_clickMarker->setLife(0.5);
      m_clickMarker->setLifeVar(0.0);
   	m_clickMarker->setStartColor(startColor);
      m_clickMarker->setStartColorVar(varColor);
		m_clickMarker->setEndColor(endColor);
      m_clickMarker->setEndColorVar(varColor);
      m_mapLayer->addChild(m_clickMarker);

      // create the HUD
      m_state = new GameState;
      m_state->setHumanCount( m_HumanSwarm.size() );
      m_state->setZombieCount( m_ZombieSwarm.size() );

      m_hud = new GameHUD;
      m_hud->initWithGame( m_state );
      addChild(m_hud);

      scheduleUpdate();

      bRet = true;
   } while (0);

   return bRet;
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

////////////////////////////////////////////////////////////////////////////////

bool SwarmGame::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
   m_panOccurred = false;
   m_lastTouchPoint = touch->locationInView( touch->view() );
   return true;
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
/*
   // DEPRECATED PANNING CODE.
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   CCPoint pt = touch->locationInView( touch->view() );
   static const float panEpsilon = 3.0;

   // only enable panning if we're not showing the full map
   if ( !m_showFullMap )
   {
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
         m_panOccurred = true;
      }
   }

   m_lastTouchPoint = pt;
*/
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
   // put target creation/update here
   if ( !m_panOccurred )
   {
      static int entityId = 5000;
      //CCPoint pt = touch->locationInView( touch->view() );
      //pt = CCDirector::sharedDirector()->convertToGL(pt);
      CCPoint pt = m_mapLayer->convertTouchToNodeSpace(touch);
      
      //pt = convertTouchToNodeSpace(touch);
      // put target creation/update here
      //ZombieBase* target = new ZombieBase(++entityId);
      int xPos, yPos;
      xPos = (int)pt.x/c_blockSize;
      yPos = (int)(m_mapLayer->getContentSize().height - pt.y)/c_blockSize;
      CCLOG("Click at %d:%d", xPos, yPos);
      MapNode* node = m_map.nodeAt(xPos, yPos);
	  assert (NULL != node);
      EntityBase* target = node->getStationaryEntity();
      target->setPos(xPos, yPos);
      m_ZombieSwarm.setTargetOfGod( target );

      m_clickMarker->setPosition(pt);
      m_clickMarker->setIsVisible(true);
      m_clickMarker->setPosition(pt);
      m_clickMarker->setIsVisible(true);
   }
   m_lastTouchPoint = ccp(-1, -1);
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::update(ccTime dt)
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   CCSize mapSize = m_mapLayer->getContentSize();
   int mapHeight = m_map.height() * c_blockSize;

   // allow the swarms to process their next move
   m_ZombieSwarm.step( float(dt) );
   m_HumanSwarm.step( float(dt) );
   // update the position of the sprites
   SpriteVector::iterator it = m_entitySprites.begin();//, limit = m_entitySprites.end();
   while ( it != m_entitySprites.end() )
   {
      // convert fallen humans to zombies
      if ( it->m_entity->isHuman ())
      {
         HumanBase* human = static_cast<HumanBase*>(it->m_entity);
         if (human->type() == EntityTank && human->isDead())
         {
            m_map.removeEntity<MobileEntity>(human);
            m_ZombieSwarm.removeTarget(human);
            m_HumanSwarm.removeEntity(human, true);
            m_state->setHumanCount( m_state->humanCount()-1);            
            m_mapLayer->removeChild(it->m_sprite, true);
            it = m_entitySprites.erase(it);
            continue;
         }
         else if (human->isZombie())
         {
            transformToZombie(*it);

            m_state->setHumanCount( m_state->humanCount()-1 );
            m_state->setZombieCount( m_state->zombieCount()+1 );
         }
      }
      else //remove dead zombies
      {
         ZombieBase* zombie = static_cast<ZombieBase*>(it->m_entity);
         if (zombie->isDead())
         {
            m_map.removeEntity<MobileEntity>(zombie);
            m_HumanSwarm.removeTarget(zombie);
            m_ZombieSwarm.removeEntity(zombie, true);
            m_state->setZombieCount( m_state->zombieCount()-1);            
            m_mapLayer->removeChild(it->m_sprite, true);
            it = m_entitySprites.erase(it);
            continue;
         }
      }

      // update the health bars on the entities
      it->m_sprite->setHealth( it->m_entity->hitPoints() );

      // update entity positions
      if ( it->m_entity->hasMoved() )
      {
         CCPoint newPos = CCPoint( float(it->m_entity->x() * c_blockSize + c_blockSize/2), float(mapHeight - ((it->m_entity->y()+1) * c_blockSize - c_blockSize/2)));

         if ( it->m_sprite->numberOfRunningActions() == 0 )
         {
            it->m_sprite->runAction(CCRotateTo::actionWithDuration(0.1f, static_cast<float>(it->m_entity->moveDirection())));
            it->m_sprite->runAction( CCMoveTo::actionWithDuration( it->m_entity->moveSpeed(), newPos ) );
         }
      }

      createDamageIndicator(*it);

      ++it;
   }

   // refresh the HUD
   m_hud->refreshFromState(dt);

   // scale out according to size of the zombie swarm
   if ( m_mapLayer->numberOfRunningActions() == 0 )
   {
      float newScale = 0.9f - m_ZombieSwarm.size() * 0.02f;
      if ( newScale < 0.1f )
         newScale = 0.2f;
      m_mapLayer->runAction( CCScaleTo::actionWithDuration(0.25f, newScale) );
   }

   // focus the view on the center of the bounding rectangle of the zombie swarm
   focusOnSwarmCenter();

   addVisibleHumanTargetsToZombieSwarm();
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::transformToZombie(EntitySprite& entity)
{
   EntityType toType;
   if (transformToType(entity.m_entity->type(), toType))
   {
      MobileEntity* zombie = static_cast<MobileEntity*>(EntityFactory::instance().createEntity(toType));
      if ( NULL != zombie  )
      {
         m_ZombieSwarm.addEntity(zombie);
         CCTexture2D* entityTexture = NULL;
         CCTextureCache* textureCache = CCTextureCache::sharedTextureCache();
         entityTexture = textureCache->textureForKey(xmlDataManager::instance().getEntitySprite (toType));
         int mapHeight = m_map.height() * c_blockSize;

         //create new sprite
         SpriteEntity* sprite = SpriteEntity::spriteWithEntity(zombie);
         sprite->setAnchorPoint(entity.m_sprite->getAnchorPoint());
         sprite->setScaleX(entity.m_sprite->getScaleX());
         sprite->setScaleY(entity.m_sprite->getScaleY());
         sprite->setPosition(entity.m_sprite->getPosition());

         m_mapLayer->removeChild(entity.m_sprite, true);
         entity.m_sprite = sprite; 
         entity.m_sprite->setMaxHealth( zombie->hitPoints() );
         entity.m_sprite->setIsVisible(true);
         m_mapLayer->addChild(entity.m_sprite);

         //TODO: add more properties
         m_map.removeEntity(entity.m_entity);
         m_map.placeEntity(zombie, entity.m_entity->x(), entity.m_entity->y());

         m_ZombieSwarm.removeTarget(entity.m_entity);
         m_HumanSwarm.removeEntity(entity.m_entity, true);
         //m_map->removeEntity(entity.m_entity);
         m_HumanSwarm.addTarget (zombie);

         entity.m_entity = zombie;         
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

bool SwarmGame::transformToType(const EntityType& fromType, EntityType& toType) const
{
   bool bResult = false;
   std::map<EntityType, int> transData;
   if (xmlDataManager::instance().getTransformationData(fromType, transData))
   {
      int probability = rand() % 100 + 1;
      std::map<EntityType, int>::const_iterator it(transData.begin()), end(transData.end());
      int value = 0;
      for (; it != end; ++it)
      {
         value += it->second;
         if (probability <= value)
         {
            toType = it->first;
            bResult = true;
            break;
         }
      }
   }
   return bResult;
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::initialiseSwarm (EntityType type, int numEntities)
{
   CCTextureCache* textureCache = CCTextureCache::sharedTextureCache();
   CCSize size( float(m_map.width()), float(m_map.height()) );
   EntityFactory& factory = EntityFactory::instance();
   MobileEntity* entity = NULL, *mapEntity = NULL;
   MapNode* node = NULL;
   int x, y;
   int mapHeight = m_map.height() * c_blockSize;

   for ( int i = 0; i < numEntities; i++ )
   {
      if ( (entity = static_cast<MobileEntity*>(factory.createEntity (type))) )
      {
         // random position
         x = rand() % static_cast<int>(size.width);
         y = rand() % static_cast<int>(size.height);

         if ( (node = m_map.nodeAt(x, y)) )
         {
            if ( node->isBlocking() )
            {
                  // if we chose a spot that already has an entity, try again...
                  i--;
                  delete entity;
                  continue;
            }
         }

         // if we get here, we can place the new entity
         m_map.placeEntity<MobileEntity>(entity, x, y);
                  
         // add it to the swarm
         CCTexture2D* entityTexture = NULL;
         if (entity->isHuman())
            m_HumanSwarm.addEntity(entity);
         else
         {
            m_ZombieSwarm.addEntity(entity);
            m_HumanSwarm.addTarget(entity);
         }
         
         // create an entity sprite structure for the visualisation
         EntitySprite es;
         
         entityTexture = textureCache->textureForKey(xmlDataManager::instance().getEntitySprite (entity->type()));
         
         es.m_entity = dynamic_cast<MobileEntity*>(entity);         
         es.m_sprite = SpriteEntity::spriteWithEntity(entity);
         es.m_sprite->setAnchorPoint( ccp(0.5, 0.5) );
         es.m_sprite->setScaleX( c_blockSize/es.m_sprite->getContentSize().width );
         es.m_sprite->setScaleY( c_blockSize/es.m_sprite->getContentSize().height );
         es.m_sprite->setPosition( ccp(es.m_entity->x() * c_blockSize + c_blockSize/2, mapHeight - ((es.m_entity->y()+1) * c_blockSize + c_blockSize/2)) );

         es.m_sprite->setMaxHealth( entity->hitPoints() );

         m_mapLayer->addChild(es.m_sprite);
         m_entitySprites.push_back(es);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::createVisibilityField()
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   CCSprite* gradientLayer = CCSprite::spriteWithFile("visfield.png");

   gradientLayer->setAnchorPoint(ccp(0.5f, 0.5f));
   gradientLayer->setPosition( ccp(winSize.width/2, winSize.height/2) );

   CCFiniteTimeAction* scaleDown = CCScaleTo::actionWithDuration(0.75f, 1.2f, 1.2f);
   CCFiniteTimeAction* scaleUp = CCScaleTo::actionWithDuration(1.0f, 1.4f, 1.4f);
   CCSequence* throb = CCSequence::actionOneTwo( scaleUp, scaleDown );

   gradientLayer->runAction( CCRepeatForever::actionWithAction( throb ));
   addChild(gradientLayer);
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::focusOnSwarmCenter()
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   CCSize mapSize = m_mapLayer->getContentSize();
   CCPoint mapPt = m_mapLayer->getPosition();
   Position pointToLookAt = m_ZombieSwarm.centerPoint();
   CCPoint pt( float(pointToLookAt.x()), float(m_map.height() - pointToLookAt.y()));

   mapSize.width *= m_mapLayer->getScale();
   mapSize.height *= m_mapLayer->getScale();

   // calculate the new position of the map
   pt.x *= c_blockSize * m_mapLayer->getScale();
   pt.y *= c_blockSize * m_mapLayer->getScale();

   // horizontal offset
   if ( pt.x + mapPt.x > winSize.width / 2 || pt.x + mapPt.x < winSize.width/2 )
      mapPt.x -= (pt.x + mapPt.x) - winSize.width/2;

   // vertical offset
   if ( pt.y + mapPt.y > winSize.height/2 || pt.y + mapPt.y < winSize.height/2 )
      mapPt.y -= (pt.y + mapPt.y) - winSize.height/2;

   // clamp the position
   if ( mapPt.x > 0 )            mapPt.x = 0;
   if ( mapPt.y > 0 )            mapPt.y = 0;
   if ( mapPt.x + mapSize.width < winSize.width )
      mapPt.x = winSize.width - (mapSize.width );
   if ( mapPt.y + mapSize.height < winSize.height )
      mapPt.y = winSize.height - (mapSize.height);

   static CCPoint lastMapPt = CCPoint(-1, -1);

   if ( lastMapPt.x != mapPt.x || lastMapPt.y != mapPt.y )
   {
      m_mapLayer->runAction( CCMoveTo::actionWithDuration( 0.5f, mapPt ) );
      lastMapPt = mapPt;
   }
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::addVisibleHumanTargetsToZombieSwarm()
{
   CCSize winSize = CCDirector::sharedDirector()->getWinSize();
   float blockSize = c_blockSize * m_mapLayer->getScale();

   // detect if any humans have entered the visible playing area
   CCPoint mapPt = m_mapLayer->getPosition();
   CCSize blockVisible( std::ceil(winSize.width/blockSize), std::ceil(winSize.height/blockSize));
   std::vector< EntityBase* > humanTargets;
   
   mapPt.x = std::abs(floor(mapPt.x / blockSize));
   mapPt.y = m_map.height() - std::abs(ceil(mapPt.y / blockSize)) - blockVisible.height;

   Rect<unsigned int> visibleMapRect(static_cast<unsigned int>(mapPt.x), static_cast<unsigned int>(mapPt.y), static_cast<unsigned int>(blockVisible.width), static_cast<unsigned int>(blockVisible.height));
   m_HumanSwarm.entitiesInRect(visibleMapRect, humanTargets);
   m_ZombieSwarm.setTargets(humanTargets);
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::createDamageIndicator(const EntitySprite& es)
{
   int damageTaken = es.m_entity->mobileStats().lastReducedHitPoints();

   if ( damageTaken )
   {
      CCPoint startingPos( es.m_sprite->getPosition().x + es.m_sprite->getContentSize().width/2, es.m_sprite->getPosition().y + es.m_sprite->getContentSize().height );
      std::stringstream ss;
      CCLabelTTF* lblDmg = NULL;
      
      ss << (damageTaken < 0 ? "+" : "-") << damageTaken;
      lblDmg = CCLabelTTF::labelWithString(ss.str().c_str(), "Arial", 12);
      lblDmg->setColor( ccc3(255, 0, 0) );
      startingPos.x -= lblDmg->getContentSize().width/2;
      lblDmg->setPosition( startingPos );
      lblDmg->setAnchorPoint( ccp(0,0) );
      m_mapLayer->addChild(lblDmg);

      CCSequence* floater = CCSequence::actionOneTwo( CCMoveTo::actionWithDuration( 1.0f, CCPoint( startingPos.x, startingPos.y + 50 ) ), CCCallFuncN::actionWithTarget( this, callfuncN_selector(SwarmGame::onDamageFloaterDone) ) );
      lblDmg->runAction( floater );

      es.m_entity->mobileStats().reset();

      // if the sprite is taking damage, then colour it red
      es.m_sprite->setColor(ccc3(255, 128, 128));
   }
   else
   {
      // if not damage is taken, reset the tint
      es.m_sprite->setColor(ccc3(255, 255, 255));
   }
}

////////////////////////////////////////////////////////////////////////////////

void SwarmGame::onDamageFloaterDone(CCNode* node)
{
   if ( node )
   {
      node->removeFromParentAndCleanup(true);
   }
}