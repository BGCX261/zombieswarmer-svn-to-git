#include "Weapon.h"
#include "xmlDataLoader.h"
#include <map>
#include <memory>
#include <assert.h>

namespace {
   typedef std::map<WeaponType, std::shared_ptr<Weapon> > WeaponMap;
   WeaponMap sWeapons;
}

std::shared_ptr<Weapon> WeaponFactory::GetWeapon(WeaponType type)
{
   WeaponMap::const_iterator find = sWeapons.find(type);
   std::shared_ptr<Weapon> weapon = NULL;
   if (find == sWeapons.end())
   {
      WeaponAttributes wa;
      if (xmlDataManager::instance().getWeaponAttributes(type, wa))
         weapon.reset (new Weapon(wa));
      else
      {
         switch (type)
         {
         case WeaponTypeFists:                  weapon.reset(new WeaponBigZombieFists());    break;
         case WeaponTypeBigZombieFists:         weapon.reset(new WeaponZombieFists());       break;
         case WeaponTypePistol:                 weapon.reset(new WeaponPistol());            break;
         case WeaponTypeShotgun:                weapon.reset(new WeaponShotgun());           break;
         case WeaponTypeMachineGun:             weapon.reset(new WeaponMachineGun());        break;
         case WeaponTypeFlameThrower:           weapon.reset(new WeaponFlameThrower());      break;
         case WeaponTypeMechFists:              weapon.reset(new WeaponMechFists());         break;
         case WeaponTypeHumanFists:             weapon.reset(new WeaponHumanFists());        break;
         case WeaponTypeHumanFatFists:          weapon.reset(new WeaponHumanFatFists());     break;
         case WeaponTypeCannon:                 weapon.reset(new WeaponCannon());            break;
         }
      }      

      assert (NULL != weapon.get());
      if (NULL != weapon)
         //sWeapons.insert(std::pair<WeaponType, std::shared_ptr<Weapon> >(type, weapon));
         sWeapons[type] = weapon;
   } 
   else
   {
      weapon = find->second;
   }

   return weapon;
}