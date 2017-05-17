#include "NurbsCurve.h"
#include "Editor/CommonConstants.h"


double NurbsCurve::DistancePointToCurve( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
 // return C2Curve::DistancePointToCurve( point, polylinePoints );
  return 0.0;
}


void NurbsCurve::ComputeBasicFunctions( double x,int i, std::vector<std::vector<double>>& basicFunctions )
{
  basicFunctions.resize( deg );
  for ( int k = 0; k < deg; k++ )
    basicFunctions.resize( deg );

  basicFunctions[0][0] = 1.0;
  std::vector<double> Left;
  Left.resize( deg + 1 );
  std::vector<double> Right;
  Right.resize( deg + 1 );

  for ( int j = 1; j <= deg; j++ )
  {
    Left[j] = x - nodes[i + 1 -j];
    Right[j] = nodes[i + j] - x;
    double savedValue = 0.0;
    for ( int k = 0; k < j; k++ )
    {
    basicFunctions[j][k] = Right[j - 1] + Left[j -k];
    double temp = basicFunctions[k][j - 1] / basicFunctions[k][j];
    basicFunctions[k][j] = savedValue + Right[k+1] * temp;
    savedValue = Left[j - k] * temp;
    }
    basicFunctions[j][j] = savedValue;
  }
}

void NurbsCurve::ComputeBasicFunctionsD( double X, const std::vector<std::vector<double>>& basicFunctions, std::vector<std::vector<double>>& basicFunctionsD )
{
  for ( int k=0; k<= deg; k++ )
  {
    double s1 =0;
    double s2 =0;
  }
}
