#include "../Entities/Entities.h"
#include "SpriteEntity.h"
#include "../Entities/xmlDataLoader.h"

SpriteEntity::SpriteEntity()
: m_healthBar( new HealthBar )
{
   m_healthBar->setPosition( ccp(0, 0) );
   m_healthBar->setSize( CCSize(30, 3) );

   addChild(m_healthBar);
}

////////////////////////////////////////////////////////////////////////////////

SpriteEntity::~SpriteEntity(void)
{
   m_healthBar->removeFromParentAndCleanup(false);
   delete m_healthBar;
}

////////////////////////////////////////////////////////////////////////////////

SpriteEntity* SpriteEntity::spriteWithEntity(EntityBase* entity)
{
   CCTextureCache* cache = CCTextureCache::sharedTextureCache();
   SpriteEntity* sprite = new SpriteEntity;

   if (entity->type())
   {
      sprite->initWithTexture( cache->textureForKey( xmlDataManager::instance().getEntitySprite (entity->type())) );
      sprite->m_healthBar->setPosition( ccp(0, 0) );
   }
   else
   {
      delete sprite;
      sprite = NULL;
   }

   return sprite;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteEntity::setRotation(float fRotation)
{
   CCSprite::setRotation(fRotation);
}