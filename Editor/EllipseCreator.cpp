#include "EllipseCreator.h"
#include "Math/Ellipse.h"
#include <cmath>


//------------------------------------------------------------------------------
// \ru Создает эллипс по центру и двум граничным точкам
// ---
std::shared_ptr<GeometricPrimitive> EllipseCreator::Create( const std::vector<Point>& points ) const
{
  Point center( points[0] );
  Point pointV( points[1] );
  // главная ось
  double r1 = sqrt( (points[0].GetX() - points[1].GetX()) * (points[0].GetX() - points[1].GetX()) +
      (points[0].GetY() - points[1].GetY()) * (points[0].GetY() - points[1].GetY()) );

  double r2 = sqrt( (points[0].GetY() - points[2].GetY()) * (points[0].GetY() - points[2].GetY()) /
      (1 - (points[0].GetX() - points[2].GetX()) * (points[0].GetX() - points[2].GetX()) / r1 * r1 ));

  Point pointX( center.GetX(), points[1].GetY() );
  double x = (pointV.GetX() - center.GetX());
  double y = (pointV.GetY() - center.GetY());
  double alpha = 0;
  if ( abs(x) < 0.01 && y <= 0 )
    alpha = -90.0;
  if ( abs(x) <0.01 == 0 && y > 0  )
    alpha = 90.0;
  if ( x > 0 && y >= 0)
    alpha = atan(y/x) * 180.0 / 3.14;
  if ( x < 0 && y >= 0)
    alpha = 3.14 + atan(y/x) * 180.0 / 3.14;
  if ( x < 0 && y <= 0)
    alpha = 3.14 + atan(y/x) * 180.0 / 3.14;
  if ( x > 0 && y <= 0)
    alpha = 2 * 3.14 + atan(y/x) * 180 / 3.14;


 return std::make_shared<Ellipse>( center, r1, r2, alpha );

}
