#include "PointCreator.h"
#include "Math/GeometricPoint.h"


//-----------------------------------------------------------------------------
/**
  \ru создание геометрического представления точки
*/
//-----------------------------------------------------------------------------
std::shared_ptr<GeometricPrimitive> PointCreator::Create( const std::vector<Point>& points ) const
{
  return std::make_shared<GeometricPoint> ( points[0] );
}
