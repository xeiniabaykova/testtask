#include "GeomPolylineCreator.h"
#include <Math/GeomPolyline.h>


//------------------------------------------------------------------------------
// \ru создание полилинии по точкам:
// точки - вершины полинии
// ---
std::shared_ptr<GeometricPrimitive> GeomPolylineCreator::Create( const std::vector<Point>& points ) const
{
  return std::make_shared <GeomPolyline>( points );
}
