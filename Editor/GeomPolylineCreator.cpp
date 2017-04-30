#include "GeomPolylineCreator.h"
#include <Math/GeomPolyline.h>
#include <QtCharts/QLineSeries>


//------------------------------------------------------------------------------
// \ru создание полилинии по точкам
// ---
std::shared_ptr<GeometricPrimitive> GeomPolylineCreator::Create( const std::vector<Point>& points )
{
  return std::make_shared <GeomPolyline>( points );
}
