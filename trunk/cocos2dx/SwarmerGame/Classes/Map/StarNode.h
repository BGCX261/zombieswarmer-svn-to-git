#ifndef _STARNODE_H_
#define _STARNODE_H_

#include <bitset>

static const unsigned int kMaxNeighborCount = 8;
static const unsigned int sNeighborList[kMaxNeighborCount][2] =
{
   {1, 0},
   {0, 1},
   {-1, 0},
   {0, -1},

   //diagonal
   {-1, -1},
   {1, -1},
   {1, 1},
   {-1, 1},
};

class StarNode
{
public:
   int x, y;
   int f;   // the movement cost to move from the starting point to destination point, following the path generated to get there
   int h;   // the estimated movement cost to move from the starting point to the destination
   int g;   // the exact cost to reach this node from the starting node
   const StarNode* parent;
};

class StarNodes
{
public:
   static const unsigned int kMaxNodes = 1024;

   StarNodes();

   StarNode* createOpenNode();
   const StarNode* getBestNode() const;
   void closeNode(const StarNode* node);
   void openNode(const StarNode* node);
   bool isInList(unsigned int x, unsigned int y) const;
   StarNode* getNodeInList(unsigned int x, unsigned int y);
   inline bool hasOpenNodes() const { return m_openNodes.any(); }
   StarNode* getStartNode();
   void reset();
   inline unsigned int getCurrentNodeIndex() const { return m_curNode; }

private:
   inline unsigned int getIndex(const StarNode* node) const { return node - &m_nodes[0]; }

   StarNode m_nodes[kMaxNodes];
   std::bitset<kMaxNodes> m_openNodes;
   unsigned int m_curNode;
   StarNode* m_startNode;
};

#endif _STARNODE_H_