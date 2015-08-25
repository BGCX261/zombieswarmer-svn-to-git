#ifndef _SPRITEENTITY_H_
#define _SPRITEENTITY_H_

#include "cocos2d.h"
#include "../Entities/EntityBase.h"
#include "HealthBar.h"

class SpriteEntity : public cocos2d::CCSprite
{
public:
    SpriteEntity();
    ~SpriteEntity(void);

    static SpriteEntity* spriteWithEntity(EntityBase* entity);

    void setEntity(EntityBase* entity)                   { m_entity = entity; }
    EntityBase* entity() const                           { return m_entity; }

    void setMaxHealth(int maxHealth)                    { m_healthBar->setMaxValue(maxHealth); }
    void setHealth(int health)                          { m_healthBar->setValue(health); }

    void setRotation(float fRotation);

private:

//    cocos2d::CCSprite*          m_sprite;
    HealthBar*                  m_healthBar;
    EntityBase*                 m_entity;
};

#endif//_SPRITEENTITY_H_