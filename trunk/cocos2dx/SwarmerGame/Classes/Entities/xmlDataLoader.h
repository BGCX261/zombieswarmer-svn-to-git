#ifndef _XML_DATA_MANAGER_H__
#define _XML_DATA_MANAGER_H__

#include "tinyxml/tinyxml.h"
#include "Entities.h"
#include "MobileProperties.h"
#include "Weapon.h"
#include <hash_map>
#include <map>
#include <string>
#include <sstream>

template <typename T> 
std::string tostr(const T& t) 
{ 
   std::ostringstream os; 
   os<<t; 
   return os.str(); 
}

class xmlDataManager
{
public:
	~xmlDataManager () {}

   static xmlDataManager& instance ();

   void setFileName (std::string f)    { m_szFileName = f; }
   void loadTree ();   
   void saveTree ();

   TiXmlElement* loadEntityProperties (TiXmlElement* pNode);
   TiXmlElement* loadWeaponProperties (TiXmlElement* pNode);
   
   bool getWeaponAttributes (WeaponType type, WeaponAttributes& wa);
   bool getMobileEntityProperties (EntityType type, MobileProperties& m);
   bool getTransformationData (EntityType, std::map<EntityType, int>& data);
   bool setMobileEntityProperties (EntityType type, MobileProperties& m);
   const char * getEntitySprite (EntityType type);

   bool isTreeLoaded ()          { return m_bLoaded; }

private:
   xmlDataManager ();
   TiXmlElement* loadTransformationData (TiXmlElement* pEntityNode);

   // entity data loaded from xml
   static std::hash_map<std::string, EntityType> m_entityTypeMap;
   static std::hash_map<EntityType, MobileProperties> m_propertyMap;
   static std::hash_map<EntityType, std::map<EntityType, int> > m_transformationMap;
   
   // weapon data loaded from xml
   static std::hash_map<std::string, WeaponType> m_weaponTypeMap;
   static std::hash_map<WeaponType, WeaponAttributes> m_WeaponMap;
   
   std::string m_szFileName;
   bool m_bLoaded;
};

#endif //_XML_DATA_MANAGER_H__