#ifndef _GAMEHUD_H_
#define _GAMEHUD_H_

#include "cocos2d.h"

using namespace cocos2d;

class GameState;

class GameHUD : public CCLayer
{
public:
   GameHUD(void);
   ~GameHUD(void);

   void initWithGame(GameState* game);
   void refreshFromState(ccTime dt);

private:

   GameState*           m_game;

   CCLabelTTF*          m_lblZombieCount;
   CCLabelTTF*          m_lblHumanCount;
   CCLabelTTF*          m_lblTime;
};

#endif//_GAMEHUD_H_