#include "C2Curve.h"
#include <cmath>
#include <limits>

namespace {

double Dot( Point point1, Point point2 )
{
  return point1.GetX() * point2.GetX() + point1.GetY() * point2.GetY();
}

//-----------------------------------------------------------------------------
/**
  \ru находим квадрат растояния между двумя точками
*/
//-----------------------------------------------------------------------------
double Distance( Point point1, Point point2 )
{
  return sqrt( (point1.GetX() - point2.GetX()) * (point1.GetX() - point2.GetX()) +
         (point1.GetY() - point2.GetY()) * (point1.GetY() - point2.GetY()) );
}

//-----------------------------------------------------------------------------
/**
  \ru расстояние от точки до прямой.
  \ru QPoint first, QPoint second - начальные и конечные точки прямой
  \ru point - точка, до которой находится расстояние
*/
//-----------------------------------------------------------------------------
double Distance( Point first, Point second, Point point )
{
   Point v = second - first;
   Point w = point - first;

   double c1 = Dot( w, v );
   if ( c1 <= 0 )
       return Distance( point, first );

   double c2 = Dot( v, v );
   if ( c2 <= c1 )
       return Distance( point, second );

   double b = c1 / c2;
   Point Pb = first + v * b;
   return Distance( point, Pb );
}
}


double C2Curve::DistancePointToCurve ( Point point, const std::vector<Point>& polylinePoints) const
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

std::vector<Point> C2Curve::GetReferensedPoints() const
{
  return referencedPoints;
}

void C2Curve::SetReferensedPoints( const std::vector<Point>& points )
{
  referencedPoints = points;
}

double C2Curve::CountingStep( double tCurrent ) const
{

  Point firstDerivative = GetDerivativePoint( tCurrent );
  Point secondDerivative = Get2DerivativePoint( tCurrent );
  double vectorMult = firstDerivative.GetX() * secondDerivative.GetY() - firstDerivative.GetY() * secondDerivative.GetX();
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double normFirstDerivative = sqrt( firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY() );
  double multiplicationFirstDerivative = firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY();
  double deltaT = 2 * sqrt ( CommonConstantsMath::PRECISION_POLYLINE * (2 * normFirstDerivative / normVectorMult - CommonConstantsMath::PRECISION_POLYLINE / multiplicationFirstDerivative) );
  return deltaT;
}


void C2Curve::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  polyLinePoints.clear();
  double t = GetRange().GetStart();
  polyLinePoints.push_back( GetPoint(t) );
  while ( t < GetRange().GetEnd() )
  {
    t += CountingStep( t );
    Point point ( GetPoint(t) );
    polyLinePoints.push_back( point );
  }
}
