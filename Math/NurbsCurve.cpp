#include "NurbsCurve.h"

double NurbsCurve::DistancePointToCurve( Point point ) const
{
  double accuracy = 0.01;
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, accuracy );
 // return C2Curve::DistancePointToCurve( point, polylinePoints );
  return 0.0;
}
