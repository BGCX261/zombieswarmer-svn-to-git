#ifndef _MAINMENU_SCENE_H__
#define _MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuStartGame(CCObject* pSender);
    virtual void menuMapGenTest(CCObject* pSender);
    virtual void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(MainMenu);
};

#endif  // _MAINMENU_SCENE_H__