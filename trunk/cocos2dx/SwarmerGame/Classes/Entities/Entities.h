#ifndef _ENTITIES_H_
#define _ENTITIES_H_

// Any new EntityTypes must have entry added to entity map in xmlloader!!
enum EntityType
{
    EntityNone = 0,
// ZOMBIES
    EntityFatZombie,
    EntityNormalZombie,
    EntityFastZombie,
    EntitySuperZombie,
// HUMANS
    EntityFatHuman,
    EntityNormalHuman,
    EntityFastHuman,
    EntityPistolHuman,
    EntityShotgunHuman,
    EntityMachineGunHuman,
    EntityMechHuman,
    EntityTank,
// MISC
    EntityWall,
    EntityBuilding,
    EntityTree, 
    EntityRoad,
    EntityGrass,
    EntityWater,
    EntityGravel,
    EntityAmmo,
    EntityTypesTotal
};

//
// Entity Keys
#define KEY_ENTITY_WALL            "brick.png"
#define KEY_ENTITY_BUILDING        "building.png"
#define KEY_ENTITY_TREE            "tree.png"
#define KEY_ENTITY_ROAD            "road.png"
#define KEY_ENTITY_GRASS           "grass.png"
#define KEY_ENTITY_WATER           "water.png"
#define KEY_ENTITY_GRAVEL          "gravel.png"
#define KEY_ENTITY_AMMO            "ammo.png"

#define KEY_ENTITY_BUILDING_OUTER_CORNER           "building-outer-corner.png"
#define KEY_ENTITY_BUILDING_FLAT_ROOF              "building-flat-roof.png"
#define KEY_ENTITY_BUILDING_INNER_CORNER           "building-inner-corner.png"
#define KEY_ENTITY_BUILDING_EDGE                   "building-edge.png"

#endif//_ENTITIES_H_