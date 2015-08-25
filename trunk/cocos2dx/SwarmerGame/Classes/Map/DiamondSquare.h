#ifndef DIAMONDSQUARE_H_
#define DIAMONDSQUARE_H_
#include <vector>

typedef std::vector<std::vector<double> > WeightArray;

class DiamondSquare
{
public:
   DiamondSquare(int width, int height, double roughness);

   struct InitialSeed
   {
      double middleWeight;
      double edge1Weight;
      double edge2Weight;
      double edge3Weight;
      double edge4Weight;
   };

   WeightArray generate(const InitialSeed& seed);

private:
   double m_roughness;
   int m_width;
   int m_height;

   void divideGrid(WeightArray& points, double x, double y, double width, double height, double c1, double c2, double c3, double c4);

   double randomDisplace(double SmallSize);
   double rectify(double num);
   double unifRand();
};

#endif //DIAMONDSQUARE_H_

