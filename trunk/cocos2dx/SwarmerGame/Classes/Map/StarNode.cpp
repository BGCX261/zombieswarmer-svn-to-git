#include "StarNode.h"
#include <assert.h>

StarNodes::StarNodes()
{
   reset();
}

void StarNodes::reset()
{
   memset (m_nodes, sizeof(m_nodes), 0);
   m_curNode = 0;
   m_startNode = NULL;
   m_openNodes.reset();
}

StarNode* StarNodes::getStartNode()
{
   if (m_startNode == NULL)
      m_startNode = createOpenNode();

   return m_startNode;
}

StarNode* StarNodes::createOpenNode()
{
   if(m_curNode >= kMaxNodes)
      return NULL;

   unsigned int nodeIndex = m_curNode;
   ++m_curNode;
   m_openNodes[nodeIndex] = 1;
   return &m_nodes[nodeIndex];
}

const StarNode* StarNodes::getBestNode() const
{
   if(m_curNode == 0)
      return NULL;

   int bestNodeF = std::numeric_limits<int>::max();
   unsigned int bestNode = 0;
   bool bFound = false;
   
   for(unsigned int i = 0; i < m_curNode; ++i)
   {
      if(m_nodes[i].f < bestNodeF && m_openNodes[i] == 1)
      {
         bestNodeF = m_nodes[i].f;
         bestNode = i;
         bFound = true;
      }
   }

   if(bFound)
      return &m_nodes[bestNode];

   return NULL;
}

void StarNodes::closeNode(const StarNode* node)
{
   unsigned int index = getIndex(node);
   assert(index < kMaxNodes);

   m_openNodes[index] = 0;
}

void StarNodes::openNode(const StarNode* node)
{
   unsigned int index = getIndex(node);
   assert(index < kMaxNodes);

   m_openNodes[index] = 1;
}

bool StarNodes::isInList(unsigned int x, unsigned int y) const
{
   for(unsigned int i = 0; i < m_curNode; ++i)
   {
      if(m_nodes[i].x == x && m_nodes[i].y == y)
         return true;
   }

   return false;
}

StarNode* StarNodes::getNodeInList(unsigned int x, unsigned int y)
{
   for(unsigned int index = 0; index < m_curNode; ++index)
   {
      if(m_nodes[index].x == x && m_nodes[index].y == y)
         return &m_nodes[index];
   }

   return NULL;
}
