#include "BoundingBox.h"
#include "GeomPolyline.h"
#include <algorithm>

namespace Math {
BoundingBox::BoundingBox( const Curve& curve )
{
  GeomPolyline polyline;
  curve.GetAsPolyLine( polyline );
  std::vector<Point> points;
  polyline.GetReferensedPoints( points );
  double minX = std::numeric_limits<double>::max();
  double minY = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::min();
  double maxY = std::numeric_limits<double>::min();

  for ( int i = 0; i < points.size(); i++ )
  {
    minX = std::min( minX, points[i].GetX() );
    minY = std::min( minY, points[i].GetY() );
    maxX = std::max( maxX, points[i].GetX() );
    maxY = std::max( maxY, points[i].GetY() );
  }
  rangeX.SetRange( minX, maxX );
  rangeY.SetRange( minY, maxY );

}
}
