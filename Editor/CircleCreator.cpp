#include "CircleCreator.h"
#include "Math/Circle.h"
#include <cmath>

//------------------------------------------------------------------------------
// \ru Создает окружность по центру и граничной точке
// ---
std::shared_ptr<GeometricPrimitive> CircleCreator::Create( const std::vector<Point>& points )
{
  Point center( points[0] );
  double r = sqrt( (points[0].GetX() - points[1].GetX()) * (points[0].GetX() - points[1].GetX()) +
      (points[0].GetY() - points[1].GetY()) * (points[0].GetY() - points[1].GetY()) );

 return std::make_shared<Circle>( center, r );
}
