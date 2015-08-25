#ifndef _MOBILE_PROPERTIES__H_
#define _MOBILE_PROPERTIES__H_

#include <string>

namespace {
   const int kNotDestructible = -1;
   const int kDestroyed = 0;
   const int kLimitOfLineOfSight = 7;
}

struct MobileProperties
{
   MobileProperties ()
      : hitPoints (0) , lineOfSight(kLimitOfLineOfSight), walkSpeed (0), runSpeed (0), endurance (0), recovery (0)
   {
   }

   MobileProperties (int oHitPoints, int oLineOfSight, float oWalkSpeed, float oRunSpeed, float oEndurance, float oRecovery)
      : hitPoints (oHitPoints)
      , lineOfSight (oLineOfSight)
      , walkSpeed (oWalkSpeed)
      , runSpeed (oRunSpeed)
      , endurance (oEndurance)
      , recovery (oRecovery)
   {
   }

   MobileProperties (std::string type, int oHitPoints, int oLineOfSight, float oWalkSpeed, float oRunSpeed, float oEndurance, float oRecovery)
      : szEntityType (type)
      , hitPoints (oHitPoints)
      , lineOfSight (oLineOfSight)
      , walkSpeed (oWalkSpeed)
      , runSpeed (oRunSpeed)
      , endurance (oEndurance)
      , recovery (oRecovery)
   {
   }

   std::string szEntityType;
   std::string szEntitySprite;
   int hitPoints;   // amount of damage a entity can take before it dies
   int lineOfSight; // limit of line of sight in blocks
   float walkSpeed; // the normal movement speed
   float runSpeed;  // the running speed
   float endurance; // amount of time (in seconds) that a entity can run/sprint
   float recovery;  // amount of time (in seconds) before a entity can run/sprint again

   int reduceHitPoints(int level) 
   {
      int reduce = 0;
      if (hitPoints > kDestroyed)
      {
         if (level > hitPoints) 
         {
            reduce = hitPoints; 
            hitPoints = kDestroyed;
         }
         else
         {
            reduce = level;
            hitPoints -= level;
         }
      }
      return reduce;
   }
};

#endif //_MOBILE_PROPERTIES__H_