#include "NurbsCurve.h"
#include "Editor/CommonConstants.h"

double NurbsCurve::DistancePointToCurve( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, accuracy );
 // return C2Curve::DistancePointToCurve( point, polylinePoints );
  return 0.0;
}
