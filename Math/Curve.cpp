#include "Curve.h"
#include <cmath>
#include <limits>
namespace Math {
namespace {


//-----------------------------------------------------------------------------
/**
  \ru расстояние от точки до прямой.
  \ru QPoint first, QPoint second - начальные и конечные точки прямой
  \ru point - точка, до которой находится расстояние
*/
//-----------------------------------------------------------------------------
double Distance( Point first, Point second, Point point )
{
   Vector v = second - first;
   Vector w = point - first;

   double c1 = v * w;
   if ( c1 <= 0 )
       return Distance( point, first );

   double c2 = v * v;
   if ( c2 <= c1 )
       return Distance( point, second );

   double b = c1 / c2;
   Point Pb = first + v * b;
   return Distance( point, Pb );
}
}


double Curve::DistancePointToCurve ( Point point, const std::vector<Point>& polylinePoints) const
{
  double minDistance = std::numeric_limits<double>::max();
  for ( int j = 1; j < polylinePoints.size(); j++ )
  {
    double currentDistance = Distance( polylinePoints[j - 1], polylinePoints[j], point );
    if ( currentDistance < minDistance )
      minDistance = currentDistance;
  }
  return minDistance;
}

std::vector<Point> Curve::GetReferensedPoints() const
{
  return referencedPoints;
}

void Curve::SetReferensedPoints( const std::vector<Point>& points )
{
  referencedPoints = points;
}

double Curve::CountingStep( double tCurrent, double accuracy) const
{

  Vector firstDerivative = GetDerivativePoint( tCurrent );
  Vector secondDerivative = Get2DerivativePoint( tCurrent );
  double vectorMult = firstDerivative.GetX() * secondDerivative.GetY() - firstDerivative.GetY() * secondDerivative.GetX();
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double normFirstDerivative = sqrt( firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY() );
  double multiplicationFirstDerivative = firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY();
  double deltaT = 2 * sqrt (accuracy * (2 * normFirstDerivative / normVectorMult - accuracy / multiplicationFirstDerivative) );
  return deltaT;
}


void Curve::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  polyLinePoints.clear();
  double t = GetRange().GetStart();
  polyLinePoints.push_back( GetPoint(t) );
  while ( t < GetRange().GetEnd() )
  {
    t += CountingStep( t, accuracy);
    Point point ( GetPoint(t) );
    polyLinePoints.push_back( point );
  }
}
}
