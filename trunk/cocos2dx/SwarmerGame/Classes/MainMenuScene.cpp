#include "MainMenuScene.h"
#include "SwarmGame.h"
#include "MapGenTest.h"

#include "SwarmGame.h"

using namespace cocos2d;

CCScene* MainMenu::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainMenu *layer = MainMenu::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

////////////////////////////////////////////////////////////////////////////////

bool MainMenu::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // Initialise the scene
        //////////////////////////////////////////////////////////////////////////

        CCSize size = CCDirector::sharedDirector()->getWinSize();

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(MainMenu::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // add the 'Start' menu item at the bottom of the screen
        CCMenuItemLabel* miStart = CCMenuItemLabel::itemWithLabel( CCLabelTTF::labelWithString("Start", CCSize(500, 30), CCTextAlignmentRight, "Arial", 32 ) );
        CCMenuItemLabel* miMapTest = CCMenuItemLabel::itemWithLabel( CCLabelTTF::labelWithString("Map Generation test", CCSize(500, 30), CCTextAlignmentRight, "Arial", 32 ) );

        miStart->setTarget(this, menu_selector(MainMenu::menuStartGame));
        miMapTest->setTarget(this, menu_selector(MainMenu::menuMapGenTest));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::menuWithItems(miStart, miMapTest, pCloseItem, NULL);
        pMenu->alignItemsVerticallyWithPadding(20.0);
        pMenu->setPosition( ccp( 100, size.height-100 ) );
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::spriteWithFile("bkg.jpg");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        bRet = true;
    } while (0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////

void MainMenu::menuStartGame(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene( SwarmGame::scene() );
}

////////////////////////////////////////////////////////////////////////////////

void MainMenu::menuMapGenTest(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene( MapGenTest::scene() );
}

////////////////////////////////////////////////////////////////////////////////

void MainMenu::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

