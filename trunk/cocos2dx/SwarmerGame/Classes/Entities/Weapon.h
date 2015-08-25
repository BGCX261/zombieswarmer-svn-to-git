#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <string>
#include <memory>

enum WeaponType
{
    WeaponTypeNone = 0,
    WeaponTypeFists,
    WeaponTypePistol,
    WeaponTypeShotgun,
    WeaponTypeMachineGun,
    WeaponTypeFlameThrower,
    WeaponTypeTreads,
    WeaponTypeCannon,
    WeaponTypeMechFists,
    WeaponTypeHumanFists,
    WeaponTypeHumanFatFists,
    WeaponTypeBigZombieFists,
    WeaponTypeTotal
};

struct WeaponAttributes
{
   WeaponAttributes ()
      : m_damagePerHit (0)
      , m_range (0)
      , m_rateOfFire (0.0f)
      , m_hitChance (0)
      , m_numShots (0)
      , m_blastRadius (0)
      , m_blastDamage (0)
   {   }

   WeaponAttributes(std::string name, WeaponType type, int damagePerHit, int range, float rateOfFire, int hitChance, int numShots, int blastRadius, int blastDamage)
      : m_weaponName (name)
      , m_type (type)
      , m_damagePerHit (damagePerHit)
      , m_range (range)
      , m_rateOfFire (rateOfFire)
      , m_hitChance (hitChance)
      , m_numShots (numShots)
      , m_blastRadius (blastRadius)
      , m_blastDamage (blastDamage)
   {   }


   std::string   m_weaponName;
   WeaponType    m_type;
   int           m_damagePerHit;
   int           m_range;
   float         m_rateOfFire;
   int           m_hitChance;
   int           m_numShots;
   int           m_blastRadius;
   int           m_blastDamage;
};

class Weapon
{
public:
    
    static int calculateDamage(Weapon* weapon)
    {
       if (NULL != weapon && (rand() % 100 < weapon->m_hitChance))
          return weapon->m_damagePerHit;
       return 0;
    }

    Weapon(std::string name, WeaponType type, int damagePerHit, int range, float rateOfFire, int hitChance, int numShots, int blastRadius, int blastDamage)
        : m_weaponName (name)
        , m_type (type)
        , m_damagePerHit (damagePerHit)
        , m_range (range)
        , m_rateOfFire (rateOfFire)
        , m_hitChance (hitChance)
        , m_blastRadius (blastRadius)
        , m_blastDamage (blastDamage)
    {
    }

    Weapon (WeaponAttributes w)
       : m_weaponName (w.m_weaponName)
       , m_type (w.m_type)
       , m_damagePerHit (w.m_damagePerHit)
       , m_range (w.m_range)
       , m_rateOfFire (w.m_rateOfFire)
       , m_hitChance (w.m_hitChance)
       , m_blastRadius (w.m_blastRadius)
       , m_blastDamage (w.m_blastDamage)
    {
    }

    std::string name() const                        { return m_weaponName; }
    WeaponType type() const                         { return m_type; }
    int damagePerHit() const                        { return m_damagePerHit; }
    int range() const                               { return m_range; }
    float rateOfFire() const                        { return m_rateOfFire; }
    int hitChance() const                           { return m_hitChance; }
    int blastRadius() const                         { return m_blastRadius; }
    int blastDamange() const                        { return m_blastDamage; }

private:
    // name of the weapon
    std::string     m_weaponName;

    WeaponType      m_type;

    // amount of damage the weapon does with each successful attack
    int           m_damagePerHit;

    // the max distance that the weapon can attack
    int           m_range;

    // the speed, in seconds, at which the weapon can make attacks. i.e. 1 second = 1 attack per second
    float           m_rateOfFire;

    // the change each attack will hit its target
    int             m_hitChance;


    // if the shot hits its target, other objects within X distance of the target will take blast damage (including the target)
    int           m_blastRadius;

    // The amount of blast damage applied on a successful hit.
    int           m_blastDamage;
};


class WeaponZombieFists : public Weapon
{
   friend class WeaponFactory;
   WeaponZombieFists() : Weapon("Fists", WeaponTypeFists, 1, 1, 0.2f, 100, -1, 0, 0) {}
};

class WeaponBigZombieFists : public Weapon
{
   friend class WeaponFactory;
   WeaponBigZombieFists() : Weapon("BigZombieFists", WeaponTypeBigZombieFists, 8, 1, 0.2f, 100, -1, 0, 0) {}
};

class WeaponPistol : public Weapon
{
   friend class WeaponFactory;
   WeaponPistol() : Weapon("Pistol", WeaponTypePistol, 1, 5, 0.75f, 75, 15, 0, 0) {}
};

class WeaponShotgun : public Weapon
{
   friend class WeaponFactory;
   WeaponShotgun() : Weapon("Shotgun", WeaponTypeShotgun, 3, 4, 1.25f, 90, 7, 1, 1) {}
};

class WeaponMachineGun : public Weapon
{
   friend class WeaponFactory;
   WeaponMachineGun() : Weapon("Machine Gun", WeaponTypeMachineGun, 1, 7, 0.1f, 65, 50, 0, 0) {}
};

class WeaponCannon : public Weapon
{
   friend WeaponFactory;
   WeaponCannon() : Weapon("Cannon", WeaponTypeHumanFists, 10, 12, 1.5f, 40, 20, 5, 2) {}
};

class WeaponFlameThrower : public Weapon
{
   friend class WeaponFactory;
   WeaponFlameThrower() : Weapon("Flame Thrower", WeaponTypeFlameThrower, 0, 3, 1.0f, 100, -1, 3, 1) {}
};

class WeaponMechFists : public Weapon
{
   friend class WeaponFactory;
   WeaponMechFists() : Weapon("Mech Fists", WeaponTypeMechFists, 3, 1, 0.5f, 95, -1, 0, 0) {}
};

class WeaponHumanFists : public Weapon
{
   friend WeaponFactory;
   WeaponHumanFists() : Weapon("Human Fists", WeaponTypeHumanFists, 1, 1, 0.2f, 100, -1, 0, 0) {}
};

class WeaponHumanFatFists : public Weapon
{
   friend WeaponFactory;
   WeaponHumanFatFists() : Weapon("Human Fat Fists", WeaponTypeHumanFists, 3, 1, 0.65f, 100, -1, 0, 0) {}
};


class WeaponFactory
{
public:
   static std::shared_ptr<Weapon> GetWeapon(WeaponType type);
};

#endif//_WEAPON_H_
