#include <Math/GeomPolyline.h>
#include "Math/CommonConstantsMath.h"
#include <cmath>

namespace {
bool IsEqualPoint(Point point1, Point point2)
{
  if ( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL )
    return true;
  return false;
}

// данные в эллипсе правильные, если: точки не совпадают, не лежат на одно линии
bool CorrectPolylineData( const std::vector<Point>& points )
{
  if (points.size() == 0)
    return false;

  for ( int i = 0; i < points.size(); i++ )
  for ( int j = 0; j < points.size(); j++ )
    {
      if ( i == j )
          continue;
       if ( IsEqualPoint(points[i], points[j]) )
          return false;
     }
return true;
}

}

//-----------------------------------------------------------------------------
/**
  Конструктор полилинии
*/
//-----------------------------------------------------------------------------
GeomPolyline::GeomPolyline(const std::vector<Point>& referensedPoits ):
  referencedPoints( referensedPoits )
{
  isValid = false;
  if ( CorrectPolylineData(referencedPoints) )
  {
    SetReferensedPoints( referensedPoits );
    isValid = true;
  }
}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для полинии: [0, количество точек]
*/
//-----------------------------------------------------------------------------
Range GeomPolyline::GetRange() const
{
  return Range( 0.0, referencedPoints.size() );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point GeomPolyline::GetPoint( double t ) const
{
  Point startPoint( referencedPoints[t] );
  Point derection( referencedPoints[t+1].GetX() - referencedPoints[t].GetX(),
      referencedPoints[t+1].GetY() - referencedPoints[t].GetY() );
  return Point( startPoint.GetX() + derection.GetX() * t, startPoint.GetY() + derection.GetY() * t );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную полилиннии по параметру t
*/
//-----------------------------------------------------------------------------
Point GeomPolyline::GetDerivativePoint( double t ) const
{
  Point direction( referencedPoints[t+1].GetX() - referencedPoints[t].GetX(),
      referencedPoints[t+1].GetY() - referencedPoints[t].GetY() );
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Point GeomPolyline::Get2DerivativePoint( double ) const
{
  return Point ( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для полилинии - это полилиния
*/
//-----------------------------------------------------------------------------
void GeomPolyline::GetAsPolyLine( std::vector<Point> & polyLinePoints, double ) const
{
  polyLinePoints = referencedPoints;
}

void GeomPolyline::Translation ( double xShift, double yShift )
{
  Point pointShift( xShift, yShift );
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = referencedPoints[i] + pointShift;
}

void GeomPolyline::Rotation( double alpha )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = Point( referencedPoints[i].GetX() * cos(alpha) - referencedPoints[i].GetY() * sin(alpha),
                                 referencedPoints[i].GetX() * sin(alpha) + referencedPoints[i].GetY() * cos(alpha) );
}

void GeomPolyline::Scaling( double xScaling, double yScaling )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = Point( referencedPoints[i].GetX() * xScaling, referencedPoints[i].GetY() * yScaling );
}

double GeomPolyline::DistanceToPoint ( Point point ) const
{
  std::vector<Point> polylinePoints;
  //GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}
bool GeomPolyline::IsValid()
{
	return isValid;
}
