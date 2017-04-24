#include "EllipseCreator.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include "Math/Ellipse.h"
#include <QtWidgets/QMessageBox>


//------------------------------------------------------------------------------
// \ru Создает эллипс по центру и двум граничным точкам
// ---
std::shared_ptr<GeometricPrimitive> EllipseCreator::Create( const std::vector<Point>& points )
{
  Point center( points[0] );
  double r1 = sqrt( (points[0].GetX() - points[1].GetX()) * (points[0].GetX() - points[1].GetX()) +
      (points[0].GetY() - points[1].GetY()) * (points[0].GetY() - points[1].GetY()) );

  double r2 = sqrt( (points[0].GetX() - points[2].GetX()) * (points[0].GetX() - points[2].GetX()) +
      (points[0].GetY() - points[2].GetY()) * (points[0].GetY() - points[2].GetY()) );

 return std::make_shared<Ellipse>( center, r1, r2 );

}
