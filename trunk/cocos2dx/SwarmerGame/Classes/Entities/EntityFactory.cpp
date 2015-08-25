#include "EntityFactory.h"

#include "Entities/Humans/FatHuman.h"
#include "Entities/Humans/FastHuman.h"
#include "Entities/Humans/NormalHuman.h"
#include "Entities/Humans/MachineGunHuman.h"
#include "Entities/Humans/MechHuman.h"
#include "Entities/Humans/PistolHuman.h"
#include "Entities/Humans/ShotgunHuman.h"
#include "Entities/Humans/Tank.h"

#include "Entities/Zombies/FatZombie.h"
#include "Entities/Zombies/FastZombie.h"
#include "Entities/Zombies/NormalZombie.h"
#include "Entities/Zombies/SuperZombie.h"
#include "StaticEntity.h"
#include "ContainerEntity.h"

EntityFactory::EntityFactory(void)
: m_lastEntityID(0)
{
}

////////////////////////////////////////////////////////////////////////////////

EntityFactory::~EntityFactory(void)
{
}

////////////////////////////////////////////////////////////////////////////////

EntityFactory& EntityFactory::instance()
{
    static EntityFactory ef;
    return ef;
}

////////////////////////////////////////////////////////////////////////////////

EntityBase* EntityFactory::createEntity (EntityType type)
{
   EntityBase* entity = NULL;
   switch (type)
   {
      // HERE BE ZOMBieZ
   case EntityFatZombie:
      entity = createEntity<FatZombie> ();
      break;
   case EntityNormalZombie:
      entity = createEntity<NormalZombie> ();
      break;
   case EntityFastZombie:
      entity = createEntity<FastZombie> ();
      break;
   case EntitySuperZombie:
      entity = createEntity<SuperZombie> ();
      break;

      // HUMANS
   case EntityFatHuman:
      entity = createEntity<FatHuman> ();
      break;   
   case EntityFastHuman:
      entity = createEntity<FastHuman> ();
      break;   
   case EntityNormalHuman:
      entity = createEntity<NormalHuman> ();
      break;
   case EntityMachineGunHuman:
      entity = createEntity<MachineGunHuman> ();
      break;   
   case EntityMechHuman:
      entity = createEntity<MechHuman> ();
      break;   
   case EntityPistolHuman:
      entity = createEntity<PistolHuman> ();
      break;   
   case EntityShotgunHuman:
      entity = createEntity<ShotGunHuman> ();
      break;   
   case EntityTank:
      entity = createEntity<Tank> ();
      break;   

      // MAP RELATED ENTITIES
   case EntityWall:
      entity = createEntity< StaticEntity< EntityWall> >();

   case EntityBuilding:
      entity = createEntity< StaticEntity< EntityBuilding> >();

   case EntityTree:
      entity = createEntity< StaticEntity< EntityTree> >();

   case EntityRoad:
      entity = createEntity< StaticEntity< EntityRoad> >();
      break;

   case EntityGrass:
      entity = createEntity< StaticEntity< EntityGrass> >();
      break;

   case EntityWater:
      entity = createEntity< StaticEntity< EntityGrass> >();
      break;

   case EntityGravel:
      entity = createEntity< StaticEntity< EntityGravel> >();
      break;

   case EntityAmmo:
      entity = createEntity<ContainerEntity>(EntityAmmo);
      break;

   case EntityNone:
   default:
      break;
   }

   return entity;
}
