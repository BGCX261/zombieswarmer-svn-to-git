#include <iostream>
#include "MapLayer.h"
#include "../Map/GameMap.h"
#include "../Map/MapNode.h"
#include "../Entities/Entities.h"
#include "../Entities/EntityBase.h"

using namespace cocos2d;

MapLayer::MapLayer(GameMap* map, unsigned int blockSize)
: m_map(map)
, m_blockSize(blockSize)
, m_textureSize(512)
, m_numPatches(0)
, m_texture(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////

MapLayer::~MapLayer(void)
{
   for ( unsigned int i = 0; i < m_numPatches; i++ )
      m_texture[i].removeFromParentAndCleanup(false);

   delete [] m_texture;
}

////////////////////////////////////////////////////////////////////////////////

bool MapLayer::init()
{
   bool bRet = false;

   do      /* enclose the body in a 'break-able' closure */
   {
      CC_BREAK_IF( !CCLayer::init() );

      if ( m_map )
      {
         m_mapSize = CCSize( static_cast<float>(m_map->width() * m_blockSize), static_cast<float>(m_map->height() * m_blockSize) );
         initWithColorWidthHeight( ccc4(32, 32, 32, 255), m_mapSize.width, m_mapSize.height );

         // generate the map's texture
         generateMapTexture();
      }

      bRet = true;
   } while (0);

   return bRet;
}

////////////////////////////////////////////////////////////////////////////////

void MapLayer::generateMapTexture()
{
   unsigned int width = m_map->width(), height = m_map->height();

   // destroy any old texture
   if ( m_texture )
   {
      for ( unsigned int i = 0; i < m_numPatches; i++ )
         m_texture[i].removeFromParentAndCleanup(false);

      delete [] m_texture;
      m_texture = NULL;
   }

   // calculate the number of horizontal and vertical patches required
   unsigned numHorizontalPatches = 1 + static_cast<unsigned int>(m_mapSize.width / m_textureSize);
   unsigned int numVerticalPatches = 1 + static_cast<unsigned int>(m_mapSize.width / m_textureSize);
   
   m_numPatches = numHorizontalPatches * numVerticalPatches;

   m_texture = new CCRenderTexture[m_numPatches];

   // edge calculations
   std::vector< MapLayer::CompoundEdge > edgesBuilding;
   int numBuildingEdges = findEdges( EntityBuilding, edgesBuilding);

   // create the texture patches
   int patchIndex = 0;
   for ( unsigned int y = 0; y < numVerticalPatches; y++ )
   {
      for ( unsigned int x = 0; x < numHorizontalPatches; x++ )
      {
         patchIndex = y * numHorizontalPatches + x;
         m_texture[patchIndex].initWithWidthAndHeight( m_textureSize, m_textureSize, kCCTexture2DPixelFormat_RGBA8888 );
         m_texture[patchIndex].setPosition( ccp(x * m_textureSize + (m_textureSize/2), m_mapSize.height - y * m_textureSize - m_textureSize/2) );

         generatePatchContent( &m_texture[patchIndex], x, y );

         addChild( &m_texture[patchIndex] );
      }
   }
   
   //
   // DEBUG CODE - render edges
   //
   /* TEMPORARY CODE  -  Edge Dump */
/*
   std::vector< MapLayer::CompoundEdge >::iterator it = edgesBuilding.begin(), limit = edgesBuilding.end();
   for ( ; it != limit; ++it )
   {
      CCLOG("Edge: [%d, %d]->[%d, %d] (%s)", it->startX, it->startY, it->endX, it->endY, 
         it->loc == CompoundEdge::Left ? "Left" : it->loc == CompoundEdge::Right ? "Right" : it->loc == CompoundEdge::Top ? "Top" : it->loc == CompoundEdge::Bottom ? "Bottom" : "None");
      float width = float( (it->endX - it->startX + 1) * m_blockSize );
      float height = float( (it->endY - it->startY + 1) * m_blockSize );
      float xPos, yPos;
      float yOffset = (it->loc == CompoundEdge::Left || it->loc == CompoundEdge::Right);

      switch ( it->loc )
      {
      case  CompoundEdge::Left:
      case  CompoundEdge::Right:
         width *= 0.1f;
         break;

      case  CompoundEdge::Top:
      case  CompoundEdge::Bottom:
         height *= 0.1f;
         break;
      }

      xPos = float( it->startX*m_blockSize );
      yPos = float( m_mapSize.height - (m_blockSize * (it->endY + yOffset)) );

      CCLayerColor* edge = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 255, 0, 255), width, height );
      edge->setAnchorPoint( ccp(0, 0) );

      switch ( it->loc )
      {
      case  CompoundEdge::Top:
         break;

      case  CompoundEdge::Right:
         xPos += m_blockSize;
         break;

      case  CompoundEdge::Bottom:
         yPos -= m_blockSize;
         break;

      case  CompoundEdge::Left:
         break;
      }

      edge->setPosition( ccp(xPos, yPos ) );
      addChild(edge);
   }
*/   

}

////////////////////////////////////////////////////////////////////////////////

void MapLayer::generatePatchContent(CCRenderTexture* patch, unsigned int patchX, unsigned int patchY)
{
   static CCTextureCache* textureCache = CCTextureCache::sharedTextureCache();
   static CCTexture2D* textures[] =
   {
      textureCache->textureForKey(KEY_ENTITY_WALL),
      textureCache->textureForKey(KEY_ENTITY_BUILDING_OUTER_CORNER),
      textureCache->textureForKey(KEY_ENTITY_BUILDING_FLAT_ROOF),
      textureCache->textureForKey(KEY_ENTITY_BUILDING_INNER_CORNER),
      textureCache->textureForKey(KEY_ENTITY_BUILDING_EDGE),
      textureCache->textureForKey(KEY_ENTITY_TREE),
      textureCache->textureForKey(KEY_ENTITY_ROAD),
      textureCache->textureForKey(KEY_ENTITY_GRASS),
      textureCache->textureForKey(KEY_ENTITY_WATER),
      textureCache->textureForKey(KEY_ENTITY_GRAVEL),
      textureCache->textureForKey(KEY_ENTITY_AMMO)
   };
   
   static const unsigned int cTextureCount = sizeof(textures) / sizeof(CCTexture2D*);

   // intialise the sprites for drawing into the texture
   CCSprite tileSprites[cTextureCount];
   ccBlendFunc blend = {GL_DST_COLOR, GL_ZERO};
   for ( int i = 0; i < cTextureCount; i++ )
   {
      tileSprites[i].initWithTexture( textures[i] );
      tileSprites[i].setAnchorPoint( ccp(0, 0) );
      tileSprites[i].setScaleX( m_blockSize/tileSprites[i].getContentSize().width );
      tileSprites[i].setScaleY( m_blockSize/tileSprites[i].getContentSize().height );
      tileSprites[i].setBlendFunc( blend ); 
      tileSprites[i].autorelease();
   }

   unsigned int startX, startY;
   unsigned int numTilesPerPatch = m_textureSize / m_blockSize;

   startX = patchX * numTilesPerPatch;
   startY = patchY * numTilesPerPatch;

   // render the content of this patch
   patch->beginWithClear( 255, 255, 255, 255 );
   {
      MapNode* node = NULL;
      EntityBase* entity = NULL;

      for ( unsigned int y = 0; y < numTilesPerPatch; y++ )
      {
         for ( unsigned int x = 0; x < numTilesPerPatch; x++ )
         {
            if ( (node = m_map->nodeAt(startX + x, startY + y)) )
            {
               if ( (entity = node->getContainerEntity()) )
               {
                  switch ( entity->type() )
                  {
                     case    EntityAmmo:
                        tileSprites[7].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                        tileSprites[7].visit();
                        break;

                     default:
                        assert(false);
                        break;
                  }
               }
               else if ( (entity = node->getStationaryEntity()) )
               {
                  switch ( entity->type() )
                  {
                  case    EntityWall:
                     tileSprites[0].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[0].visit();
                     break;

                  case    EntityBuilding:
                     {
                        tileSprites[1].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                        tileSprites[1].visit();
                     }
                     break;

                  case    EntityTree:
                     tileSprites[5].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[5].visit();
                     break;

                  case    EntityRoad:
                     tileSprites[6].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[6].visit();
                     break;

                  case    EntityGrass:
                     tileSprites[7].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[7].visit();
                     break;

                  case    EntityWater:
                     tileSprites[8].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[8].visit();
                     break;

                  case    EntityGravel:
                     tileSprites[9].setPosition( ccp( x * m_blockSize, numTilesPerPatch * m_blockSize - (y+1) * m_blockSize ) );
                     tileSprites[9].visit();
                     break;

                  default:
                     assert(false);
                     break;
                  }
               }
            }
         }
      }
   }
   patch->end();
}

////////////////////////////////////////////////////////////////////////////////

size_t MapLayer::findEdges(EntityType type, std::vector<MapLayer::CompoundEdge>& edges)
{
   size_t ret = 0;

   ret = findHorizontalEdges(type, edges) + 
         findVerticalEdges(type, edges);

   return ret;
}

////////////////////////////////////////////////////////////////////////////////

size_t MapLayer::findHorizontalEdges(EntityType type, std::vector<MapLayer::CompoundEdge>& edges)
{
   MapNode* node = NULL;
   CompoundEdge edge;
   size_t count = 0;

   for ( unsigned int y = 0; y < m_map->height(); y++ )
   {
      for ( unsigned int x = 0; x < m_map->width(); x++ )
      {
         // get the node and ensure that it has an entity 
         if ( (node = m_map->nodeAt(x, y)) )
         {
            // ensure that the entity is of the type we are looking for
            if ( node->getStationaryEntity()->type() == type )
            {
               // check if the entity above us is of our type as well...
               if ( (node = m_map->nodeAt(x, y-1)) && node->getStationaryEntity()->type() == type )
               {
                  // bottom edge detector
                  if ( (node = m_map->nodeAt(x, y+1)) && node->getStationaryEntity()->type() != type )
                  {
                     if ( edge.startX == -1 )
                     {
                        edge.startX = x;
                        edge.startY = y;
                        edge.loc = CompoundEdge::Bottom;
                     }
                  }
                  else
                  {
                     if ( edge.startX != -1 )
                     {
                        edge.endX = x-1;
                        edge.endY = y;
                        edges.push_back(edge);
                        count++;

                        edge.reset();
                     }
                  }
               }
               else
               {
                  if ( edge.startX == -1 )
                  {
                     // top edge
                     edge.startX = x;
                     edge.startY = y;
                     edge.loc = CompoundEdge::Top;
                  }
                  else
                  {
                     if ( edge.loc == CompoundEdge::Bottom )
                     {
                        edge.endX = x-1;
                        edge.endY = y;
                        edges.push_back(edge);
                        count++;

                        edge.reset();
                     }
                  }
               }
            }
            else
            {
               if ( edge.startX != -1 )
               {
                  edge.endX = x-1;
                  edge.endY = y;
                  edges.push_back(edge);
                  count++;

                  edge.reset();
               }
            }
         }
      }
   }

   return count;
}

////////////////////////////////////////////////////////////////////////////////

size_t MapLayer::findVerticalEdges(EntityType type, std::vector<MapLayer::CompoundEdge>& edges)
{
   MapNode* node = NULL;
   CompoundEdge edge;
   size_t count = 0;

   for ( unsigned int x = 0; x < m_map->width(); x++ )
   {
      for ( unsigned int y = 0; y < m_map->height(); y++ )
      {
         // get the node and ensure that it has an entity 
         if ( (node = m_map->nodeAt(x, y)) )
         {
            // ensure that the entity is of the type we are looking for
            if ( node->getStationaryEntity()->type() == type )
            {
               if ( (node = m_map->nodeAt(x-1, y)) && node->getStationaryEntity()->type() == type )
               {
                  // right edge detector
                  if ( (node = m_map->nodeAt(x+1, y)) && node->getStationaryEntity()->type() != type )
                  {
                     if ( edge.startX == -1 )
                     {
                        edge.startX = x;
                        edge.startY = y;
                        edge.loc = CompoundEdge::Right;
                     }
                  }
               }
               else
               {
                  // left edge
                  if ( edge.startX == -1 )
                  {
                     edge.startX = x;
                     edge.startY = y;
                     edge.loc = CompoundEdge::Left;
                  }
               }
            }
            else
            {
               if ( edge.startX != -1 )
               {
                  edge.endX = x;
                  edge.endY = y-1;
                  edges.push_back(edge);
                  count++;

                  if ( edge.loc == CompoundEdge::Left && edge.endY == edge.startY )
                  {
                     edge.loc = CompoundEdge::Right;
                     edges.push_back(edge);
                     count++;
                  }
                  
                  edge.reset();
               }
            }
         }
      }
   }

   return count;
}