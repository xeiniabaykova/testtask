#include "PointCreator.h"
#include "Math/GeometricPoint.h"


//-----------------------------------------------------------------------------
/**
  \ru создание геометрического представления точки
*/
//-----------------------------------------------------------------------------
std::shared_ptr<GeometricPrimitive> PointCreator::Create( const std::vector<Point>& points )
{
  return std::make_shared<GeometricPoint> ( points[0] );
}
