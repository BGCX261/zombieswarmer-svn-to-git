#include "xmlDataLoader.h"
#include <string>
#include <map>


//entity data
std::hash_map<std::string, EntityType> xmlDataManager::m_entityTypeMap;
std::hash_map<EntityType, MobileProperties> xmlDataManager::m_propertyMap;
std::hash_map<EntityType, std::map<EntityType, int> > xmlDataManager::m_transformationMap;

//weapon data
std::hash_map<std::string, WeaponType> xmlDataManager::m_weaponTypeMap;
std::hash_map<WeaponType, WeaponAttributes> xmlDataManager::m_WeaponMap;

xmlDataManager::xmlDataManager () 
   : m_szFileName ("EntityTypes.xml")
   , m_bLoaded (false)
{
   m_entityTypeMap["FatHuman"]            = EntityFatHuman;
   m_entityTypeMap["NormalHuman"]         = EntityNormalHuman;
   m_entityTypeMap["FastHuman"]           = EntityFastHuman;
   m_entityTypeMap["PistolHuman"]         = EntityPistolHuman;
   m_entityTypeMap["ShotGunHuman"]        = EntityShotgunHuman;
   m_entityTypeMap["MachineGunHuman"]     = EntityMachineGunHuman;
   m_entityTypeMap["MechHuman"]           = EntityMechHuman;
   m_entityTypeMap["Tank"]                = EntityTank;

   // HERE BE ZOMBieZ
   m_entityTypeMap["FatZombie"]           = EntityFatZombie;
   m_entityTypeMap["NormalZombie"]        = EntityNormalZombie;
   m_entityTypeMap["FastZombie"]          = EntityFastZombie;
   m_entityTypeMap["SuperZombie"]         = EntitySuperZombie;

   // WEAPONZ
   m_weaponTypeMap["Fists"]               = WeaponTypeFists;
   m_weaponTypeMap["BigZombieFists"]      = WeaponTypeBigZombieFists;
   m_weaponTypeMap["Pistol"]              = WeaponTypePistol;
   m_weaponTypeMap["Shotgun"]             = WeaponTypeShotgun;
   m_weaponTypeMap["MachineGun"]          = WeaponTypeMachineGun;
   m_weaponTypeMap["Cannon"]              = WeaponTypeCannon;
   m_weaponTypeMap["FlameThrower"]        = WeaponTypeFlameThrower;
   m_weaponTypeMap["MechFists"]           = WeaponTypeMechFists;
   m_weaponTypeMap["HumanFists"]          = WeaponTypeHumanFists;
   m_weaponTypeMap["HumanFatFists"]       = WeaponTypeHumanFatFists;

}

xmlDataManager& xmlDataManager::instance ()
{
   static xmlDataManager i;
   
   if (!i.isTreeLoaded ())
      i.loadTree();

   return i;
}

void xmlDataManager::loadTree ()
{
   m_bLoaded = true;
   TiXmlDocument document (m_szFileName.c_str());
   if (document.LoadFile())
   {
      TiXmlHandle hRoot(0);
      TiXmlElement* EntityRoot = document.FirstChildElement ("EntityTypes");
      if (NULL != EntityRoot)
      {
         hRoot = TiXmlHandle (EntityRoot);
         TiXmlElement* pNode = EntityRoot->FirstChildElement("Entity");
         if (NULL != pNode)
            pNode = loadEntityProperties(pNode);         
      }   
      
      TiXmlElement* WeaponRoot = document.FirstChildElement ("Weapons");
      if (NULL != WeaponRoot)
      {
         hRoot = TiXmlHandle (WeaponRoot);
         TiXmlElement* pNode = WeaponRoot->FirstChildElement("Weapon");
         if (NULL != pNode)
         pNode = loadWeaponProperties(pNode);
      }
   }   
}

bool xmlDataManager::getMobileEntityProperties (EntityType type, MobileProperties& m)
{
   bool bResult = false;
   
   if (type > EntityNone && type < EntityTypesTotal)
   {
      m = m_propertyMap[type];
      bResult = true;
   }

   return bResult;
}

bool xmlDataManager::getTransformationData (EntityType type, std::map<EntityType, int>& data)
{
   bool bResult = false;
   if (!m_transformationMap [type].empty())
   {
      data = m_transformationMap [type];
      bResult = true;
   }

   return bResult;
}

TiXmlElement* xmlDataManager::loadEntityProperties (TiXmlElement* pEntityNode)
{
   for (pEntityNode; pEntityNode; pEntityNode = pEntityNode->NextSiblingElement())
   {
      EntityType t;
      MobileProperties m;
      const char *pName = pEntityNode->Attribute("name");
      if (pName)
      {
         m.szEntityType = pName;
         t = m_entityTypeMap[std::string(pName)];
         
         pName = pEntityNode->Attribute("sprite");
         if (pName)
            m.szEntitySprite = pName;

         pEntityNode->QueryIntAttribute ("HitPoints",           &m.hitPoints);
         pEntityNode->QueryIntAttribute ("LineOfSightDistance", &m.lineOfSight);
         pEntityNode->QueryFloatAttribute ("WalkSpeed",         &m.walkSpeed);
         pEntityNode->QueryFloatAttribute ("RunSpeed",          &m.runSpeed);
         pEntityNode->QueryFloatAttribute ("Endurance",         &m.endurance);
         pEntityNode->QueryFloatAttribute ("Recovery",          &m.recovery);

         loadTransformationData(pEntityNode);

         m_propertyMap[t] = m;
      }
   }	
   return pEntityNode;
}

TiXmlElement* xmlDataManager::loadTransformationData (TiXmlElement* pEntityNode)
{
	const char *pName = pEntityNode->Attribute("name");
	if (pName)
	{
      TiXmlElement* pEntityChildNode = pEntityNode->FirstChildElement("TransformationProbability");
      if (NULL != pEntityChildNode)
      {
         std::map<EntityType, int> transMap;
         pEntityChildNode->QueryIntAttribute("NormalZombie", &transMap [EntityNormalZombie]);
         pEntityChildNode->QueryIntAttribute("FatZombie", &transMap [EntityFatZombie]);
         pEntityChildNode->QueryIntAttribute("FastZombie", &transMap [EntityFastZombie]);
         pEntityChildNode->QueryIntAttribute("SuperZombie", &transMap [EntitySuperZombie]);
         m_transformationMap[m_entityTypeMap[std::string(pName)]] = transMap;
      }
	}
	
	return pEntityNode;
}


TiXmlElement* xmlDataManager::loadWeaponProperties (TiXmlElement* pNode)
{
   for (pNode; pNode; pNode = pNode->NextSiblingElement())
   {
      WeaponType    t;
      WeaponAttributes w;
      
      const char *pName = pNode->Attribute("name");
      if (pName)
      {
         t = m_weaponTypeMap[std::string(pName)];
         w.m_type = t;
         w.m_weaponName = pName;

         pNode->QueryIntAttribute   ("damagePerHit",     &w.m_damagePerHit);
         pNode->QueryIntAttribute   ("range",            &w.m_range);
         pNode->QueryFloatAttribute ("rateOfFire",       &w.m_rateOfFire);
         pNode->QueryIntAttribute   ("hitChance",        &w.m_hitChance);
         pNode->QueryIntAttribute   ("numShots",         &w.m_numShots);
         pNode->QueryIntAttribute   ("blastRadius",      &w.m_blastRadius);
         pNode->QueryIntAttribute   ("blastDamage",      &w.m_blastDamage);

         m_WeaponMap[t] = w;
      }
   }
   return pNode;
}


void xmlDataManager::saveTree ()
{
   TiXmlDocument doc;  
   TiXmlComment * comment;
   std::string s;

   TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
   doc.LinkEndChild( decl ); 

   TiXmlElement * root = new TiXmlElement("EntityTypes");  
   doc.LinkEndChild( root );  

   comment = new TiXmlComment();
   s=" Settings for EntityTypes";
   comment->SetValue(s.c_str());  
   root->LinkEndChild( comment );

   std::hash_map<EntityType, MobileProperties>::iterator iter;

   for (iter=m_propertyMap.begin(); iter != m_propertyMap.end(); iter++)
   {
      MobileProperties m = (*iter).second;

      TiXmlElement * entityProperties;
      entityProperties = new TiXmlElement( "Entity" );  
      root->LinkEndChild( entityProperties );  
      
      entityProperties->SetAttribute("name",       m.szEntityType.c_str());
      entityProperties->SetAttribute("name",       m.szEntitySprite.c_str());
      entityProperties->SetAttribute("HitPoints",  m.hitPoints);
      entityProperties->SetAttribute("WalkSpeed",  tostr(m.walkSpeed).c_str());
      entityProperties->SetAttribute("RunSpeed",   tostr(m.runSpeed).c_str());
      entityProperties->SetAttribute("Endurance",  tostr(m.endurance).c_str());
      entityProperties->SetAttribute("Recovery",   tostr(m.recovery).c_str());
   }
   
   doc.SaveFile(m_szFileName.c_str());
}

bool xmlDataManager::setMobileEntityProperties (EntityType type, MobileProperties& m)
{
   bool bResult = false;

   if (type > EntityNone && type < EntityTypesTotal)
   {
      m_propertyMap[type] = m;
      bResult = true;
   }

   return bResult;
}

const char * xmlDataManager::getEntitySprite (EntityType type)
{
   return m_propertyMap[type].szEntitySprite.c_str();
}

bool xmlDataManager::getWeaponAttributes (WeaponType type, WeaponAttributes& wa)
{
   bool bResult = false;

   if (type > WeaponTypeNone && type < WeaponTypeTotal)
   {
      wa = m_WeaponMap[type];
      bResult = true;
   }

   return bResult;
}
