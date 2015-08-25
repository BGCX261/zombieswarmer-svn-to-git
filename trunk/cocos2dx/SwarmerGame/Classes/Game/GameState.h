#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

class GameState
{
public:
   GameState(void);
   virtual ~GameState(void);

   void setLevel(unsigned int level)                        { m_level = level; }
   void setHumanCount(unsigned int num)                     { m_numHumans = num; }
   void setZombieCount(unsigned int num)                    { m_numZombies = num; }
   void incrementTimeOnLevel()                              { m_timeOnLevel++; }

   unsigned int level()                                     { return m_level; }
   unsigned int humanCount()                                { return m_numHumans; }
   unsigned int zombieCount()                               { return m_numZombies; }
   unsigned int timeOnLevel()                               { return m_timeOnLevel; }

private:

   unsigned int            m_level;
   unsigned int            m_numHumans;
   unsigned int            m_numZombies;
   unsigned int            m_timeOnLevel;
};

#endif//_GAMESTATE_H_