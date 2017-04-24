#include "LineCreator.h"
#include "Math/Line.h"
#include <QtCharts/QLineSeries>


//------------------------------------------------------------------------------
// \ru создание линии по двум точкам: началу и концу
// ---
std::shared_ptr<GeometricPrimitive> LineCreator::Create( const std::vector<Point>& points )
{
  Point startPoint( points[0] );
  Point endPoint( points[1]) ;
  return std::make_shared <Line>( startPoint, endPoint );
}
