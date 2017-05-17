#include <Math/GeomPolyline.h>
#include "Editor/CommonConstants.h"
#include <cmath>

//-----------------------------------------------------------------------------
/**
  Конструктор полилинии
*/
//-----------------------------------------------------------------------------
GeomPolyline::GeomPolyline(const std::vector<Point>& referensedPoits ):
  referencedPoints( referensedPoits )
{
  SetReferensedPoints( referensedPoits );
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
Point GeomPolyline::Get2DerivativePoint( double t ) const
{
  (void)t;
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

void GeomPolyline::Dilatation( double xScaling, double yScaling )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = Point( referencedPoints[i].GetX() * xScaling, referencedPoints[i].GetY() * yScaling );
}

double GeomPolyline::DistanceToPoint ( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}
