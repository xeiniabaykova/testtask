#include "NurbsCurve.h"
#include "Editor/CommonConstants.h"

double NurbsCurve::DistancePointToCurve( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
 // return C2Curve::DistancePointToCurve( point, polylinePoints );
  return 0.0;
}
