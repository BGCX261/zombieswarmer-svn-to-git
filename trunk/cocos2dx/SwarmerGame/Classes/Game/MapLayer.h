#ifndef _MAPLAYER_H_
#define _MAPLAYER_H_

#include "cocos2d.h"
#include <vector>
#include "../Entities/Entities.h"

class GameMap;

using namespace cocos2d;

class MapLayer : public CCLayerColor
{
   struct TexturePatch
   {
      CCRenderTexture*  texture;
      CCPoint           m_pos;
   };

   struct CompoundEdge
   {
      enum EdgeLocation
      {
         None,
         Top,
         Right,
         Bottom,
         Left
      };

      CompoundEdge() : startX(-1), startY(-1), endX(-1), endY(-1), loc(None) {}
      void reset() { startX = startY = endX = endY = -1; }

      int            startX, startY;
      int            endX, endY;
      EdgeLocation   loc;
   };

public:
   MapLayer(GameMap* map, unsigned int blockSize);
   ~MapLayer(void);

   // called from the 'LAYER_NODE_FUNC()' mechanism defined below
   virtual bool init();  

private:

   void generateMapTexture();
   void generatePatchContent(CCRenderTexture* patch, unsigned int patchX, unsigned int patchY);

   size_t findEdges(EntityType type, std::vector<CompoundEdge>& edges);
   size_t findHorizontalEdges(EntityType type, std::vector<CompoundEdge>& edges);
   size_t findVerticalEdges(EntityType type, std::vector<CompoundEdge>& edges);

   GameMap*                m_map;
   unsigned int            m_blockSize;
   unsigned int            m_textureSize;
   unsigned int            m_numPatches;
   CCSize                  m_mapSize;
   CCRenderTexture*        m_texture;
};

#endif//_MAPLAYER_H_