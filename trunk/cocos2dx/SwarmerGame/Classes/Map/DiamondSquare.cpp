#include "DiamondSquare.h"

DiamondSquare::DiamondSquare(int width, int height, double roughness)
   : m_roughness(roughness)
   , m_width(width)
   , m_height(height)
{
}

WeightArray DiamondSquare::generate(const InitialSeed& seed)
{
   WeightArray points;
   points.resize(m_width + 1);

   for (int i = 0; i < m_width + 1; ++i)
      points[i].resize(m_height + 1);

   //Initial seed
   //default values are 1.0f for middle point, and 0 for the edges
   points[m_width / 2][m_height / 2]   = seed.middleWeight;
   points[0][0]                        = seed.edge1Weight;
   points[m_width][0]                  = seed.edge2Weight;
   points[0][m_height]                 = seed.edge3Weight;
   points[m_width][m_height]           = seed.edge4Weight;

   double newWidth = std::floor(static_cast<double>(m_width / 2));
   double newHeight = std::floor(static_cast<double>(m_height / 2));

   divideGrid(points, 0, 0, newWidth, newHeight, seed.edge1Weight, seed.edge1Weight, seed.middleWeight, seed.edge4Weight);
   divideGrid(points, newWidth, 0, m_width - newWidth, newHeight, seed.edge1Weight, seed.edge2Weight, seed.edge2Weight, seed.middleWeight);
   divideGrid(points, newWidth, newHeight, m_width - newWidth, m_height - newHeight, seed.middleWeight, seed.edge2Weight, seed.edge3Weight, seed.edge3Weight);
   divideGrid(points, 0, newHeight, newWidth, m_height - newHeight, seed.edge4Weight, seed.middleWeight, seed.edge3Weight, seed.edge4Weight);

   return points;
}

void DiamondSquare::divideGrid(WeightArray& points, double x, double y, double width, double height, double corner1, double corner2, double corner3, double corner4)
{
   double newWidth = std::floor(static_cast<double>(width / 2));
   double newHeight = std::floor(static_cast<double>(height / 2));

   if (width > 1 || height > 1)
   {
      double middleWeight = ((corner1 + corner2 + corner3 + corner4) / 4) + randomDisplace(newWidth + newHeight);
      double edge1Weight = ((corner1 + corner2) / 2);
      double edge2Weight = ((corner2 + corner3) / 2);
      double edge3Weight = ((corner3 + corner4) / 2);
      double edge4Weight = ((corner4 + corner1) / 2);

      //Make sure the new weights are within boundaries ([0, 1])
      middleWeight= rectify(middleWeight);
      edge1Weight = rectify(edge1Weight);
      edge2Weight = rectify(edge2Weight);
      edge3Weight = rectify(edge3Weight);
      edge4Weight = rectify(edge4Weight);

      divideGrid (points, x, y, newWidth, newHeight, corner1, edge1Weight, middleWeight, edge4Weight);
      divideGrid (points, x + newWidth, y, width - newWidth, newHeight, edge1Weight, corner2, edge2Weight, middleWeight);
      divideGrid (points, x + newWidth, y + newHeight, width - newWidth, height - newHeight, middleWeight, edge2Weight, corner3, edge3Weight);
      divideGrid (points, x, y + newHeight, newWidth, height - newHeight, edge4Weight, middleWeight, edge3Weight, corner4);
   }
   else
   {
      double corner = (corner1 + corner2 + corner3 + corner4) / 4;

      points[(int)(x)][(int)(y)] = corner;
      if (width == 2)
         points[(int)(x+1)][(int)(y)] = corner;

      if (height == 2)
         points[(int)(x)][(int)(y+1)] = corner;

      if ((width == 2) && (height == 2))
         points[(int)(x + 1)][(int)(y+1)] = corner;
   }
}

double DiamondSquare::rectify(double num)
{
   if (num < 0)
      num = 0;
   else if (num > 1.0)
      num = 1.0;

   return num;
}

double DiamondSquare::randomDisplace(double SmallSize)
{
   double Max = SmallSize/ (m_width + m_height) * m_roughness;
   return (unifRand() - 0.5) * Max;
}

double DiamondSquare::unifRand()
{
   return rand() / double(RAND_MAX);
}
