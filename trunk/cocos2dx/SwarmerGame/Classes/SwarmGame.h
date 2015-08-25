#ifndef _SWARMGAME_H_
#define _SWARMGAME_H_

#include "cocos2d.h"
#include <vector>
#include "Entities/EntitySwarm.h"
#include "Map/GameMap.h"
#include "Game/GameState.h"
#include "Game/MapLayer.h"
#include "Game/GameHUD.h"
#include "Game/SpriteEntity.h"

using namespace cocos2d;

class SwarmGame : public CCLayer
{
   struct EntitySprite
   {
      SpriteEntity*        m_sprite;
      MobileEntity*        m_entity;
   };
    
   typedef std::vector< EntitySprite >         SpriteVector;

public:
   SwarmGame(void);
   ~SwarmGame(void);

   static CCScene* scene();

   // called from the 'LAYER_NODE_FUNC()' mechanism defined below
   virtual bool init();  

   void registerWithTouchDispatcher();
   bool ccTouchBegan(CCTouch* touch, CCEvent* event);
   void ccTouchMoved(CCTouch* touch, CCEvent* event);
   void ccTouchEnded(CCTouch* touch, CCEvent* event);

   // implement the "static node()" method manually
   LAYER_NODE_FUNC(SwarmGame);

   void update(ccTime dt);

private:

   void transformToZombie(EntitySprite& entity);
   bool transformToType(const EntityType& fromType, EntityType& toType) const;
   void initialiseSwarm(EntityType type, int numEntities = 50);

   void createVisibilityField();
   void focusOnSwarmCenter();
   void addVisibleHumanTargetsToZombieSwarm();
   void createDamageIndicator(const EntitySprite& es);

   virtual void onDamageFloaterDone(CCNode* node);

   GameState*              m_state;
   GameMap                 m_map;
   MapLayer*               m_mapLayer;
   GameHUD*                m_hud;
   EntitySwarm             m_ZombieSwarm;
   EntitySwarm             m_HumanSwarm;
   SpriteVector            m_entitySprites;
   CCPoint                 m_lastTouchPoint;
   bool                    m_showFullMap;
   bool                    m_panOccurred;

   CCParticleSystem*       m_clickMarker;
};

#endif//_SWARMGAME_H_