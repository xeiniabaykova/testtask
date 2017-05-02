#include "EllipseCreator.h"
#include "Math/Ellipse.h"
#include <cmath>


//------------------------------------------------------------------------------
// \ru Создает эллипс по центру и двум граничным точкам
// ---
std::shared_ptr<GeometricPrimitive> EllipseCreator::Create( const std::vector<Point>& points )
{
  Point center( points[0] );
  Point pointV( points[1] );
  double r1 = sqrt( (points[0].GetX() - points[1].GetX()) * (points[0].GetX() - points[1].GetX()) +
      (points[0].GetY() - points[1].GetY()) * (points[0].GetY() - points[1].GetY()) );

  double r2 = sqrt( (points[0].GetX() - points[2].GetX()) * (points[0].GetX() - points[2].GetX()) +
      (points[0].GetY() - points[2].GetY()) * (points[0].GetY() - points[2].GetY()) );

    Point pointX(center.GetX(), points[1].GetY());
    double a = sqrt((center.GetX() - pointV.GetX()) * (center.GetX() - pointV.GetX()) + (center.GetY() - pointV.GetY()) *(center.GetY() - pointV.GetY()));
    double b = sqrt((center.GetX() - pointX.GetX()) * (center.GetX() - pointX.GetX()) + (center.GetY() - pointX.GetY()) *(center.GetY() - pointX.GetY()));
    double ab = center.GetX()* center.GetX() + center.GetY() * center.GetY() + pointV.GetX() * pointX.GetX() + pointV.GetY() * pointX.GetY();
    double alpha = acos( ab / a * b );

 return std::make_shared<Ellipse>( center, r1, r2 );

}
