#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


#include "MainMenuScene.h"
#include "Entities/Entities.h"
#include "Entities/xmlDataLoader.h"

using namespace cocos2d;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
   SimpleAudioEngine::end();
}

bool AppDelegate::initInstance()
{
   srand( unsigned int( time(NULL) ) );

   bool bRet = false;
   do 
   {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
      // Initialize OpenGLView instance, that release by CCDirector when application terminate.
      // The HelloWorld is designed as HVGA.
      CCEGLView * pMainWnd = new CCEGLView();
      CC_BREAK_IF(! pMainWnd
         || ! pMainWnd->Create(TEXT("SwarmerGame"), 800, 600));
#endif  // CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

      // OpenGLView initialized in testsAppDelegate.mm on ios platform, nothing need to do here.

#endif  // CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

      // Android doesn't need to do anything.

#endif  // CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
      // Initialize OpenGLView instance, that release by CCDirector when application terminate.
      // The HelloWorld is designed as HVGA.
      // Use GetScreenWidth() and GetScreenHeight() get screen width and height.
      CCEGLView * pMainWnd = new CCEGLView(this);
      CC_BREAK_IF(! pMainWnd
         || ! pMainWnd->Create(320, 480));

#if !defined(_TRANZDA_VM_)
      // set the resource zip file
      // on wophone emulator, we copy resources files to Work7/TG3/APP/ folder instead of zip file
      CCFileUtils::setResource("SwarmerGame.zip");
#endif

#endif  // CC_PLATFORM_WOPHONE

      // load the game textures into the TextureCache
      CCTextureCache* cache = CCTextureCache::sharedTextureCache();
      
      cache->addImage(KEY_ENTITY_WALL);
      cache->addImage(KEY_ENTITY_BUILDING);
      cache->addImage(KEY_ENTITY_BUILDING_OUTER_CORNER);
      cache->addImage(KEY_ENTITY_BUILDING_INNER_CORNER);
      cache->addImage(KEY_ENTITY_BUILDING_EDGE);
      cache->addImage(KEY_ENTITY_BUILDING_FLAT_ROOF);
      cache->addImage(KEY_ENTITY_ROAD);
      cache->addImage(KEY_ENTITY_GRASS);
      cache->addImage(KEY_ENTITY_WATER);
      cache->addImage(KEY_ENTITY_GRAVEL);
      cache->addImage(KEY_ENTITY_AMMO);
      cache->addImage(KEY_ENTITY_TREE);
      
      //xml defined sprites for mobilentities
      for (int entity = EntityNone+1; entity <= EntityTank; entity++)
         cache->addImage (xmlDataManager::instance().getEntitySprite ((EntityType)entity));

      bRet = true;
   } while (0);
   return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
   // initialize director
   CCDirector *pDirector = CCDirector::sharedDirector();
   pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

   // sets landscape mode
//   pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);

   // turn on display FPS
   pDirector->setDisplayFPS(true);

   // set FPS. the default value is 1.0/60 if you don't call this
   pDirector->setAnimationInterval(1.0 / 60.0);

   // create a scene. it's an autorelease object
   CCScene *pScene = MainMenu::scene();

   // run
   pDirector->runWithScene(pScene);
   return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
   CCDirector::sharedDirector()->pause();

   SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
   CCDirector::sharedDirector()->resume();

   SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
