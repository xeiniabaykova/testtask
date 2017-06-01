#include "GeomPolyline.h"
#include "CommonConstantsMath.h"
#include <cmath>

namespace Math {
namespace {

//-----------------------------------------------------------------------------
/**
  Полилиния валидна, если количество точек не равно нулю, если точки не совпадают.
*/
//---
static bool CorrectPolylineData( const std::vector<Point>& points )
{
  if ( points.size() == 0 )
    return false;

  for ( int i = 0; i < points.size(); i++ )
  for ( int j = 0; j < points.size(); j++ )
    {
      if ( i == j )
          continue;
       if ( IsEqual(points[i], points[j]) )
          return false;
     }
  return true;
}

}


///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления полилинии.
/**
  Представлет функции для хранения и проведения опраций над полилинией.
*/
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
/**
  Конструктор полилинии по опорным точкам.
*/
//---
GeomPolyline::GeomPolyline( const std::vector<Point>& referensedPoits ):
  referencedPoints( referensedPoits )
{
  if ( CorrectPolylineData(referencedPoints) )
  {
    SetReferensedPoints( referensedPoits );
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
Vector GeomPolyline::GetDerivativePoint( double t ) const
{
  Vector direction( referencedPoints[t+1].GetX() - referencedPoints[t].GetX(),
      referencedPoints[t+1].GetY() - referencedPoints[t].GetY() );
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Vector GeomPolyline::Get2DerivativePoint( double ) const
{
  return Vector ( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru Возвращается полилилния для полилинии - это полилиния.
*/
//---
void GeomPolyline::GetAsPolyLine( std::vector<Point> & polyLinePoints, double ) const
{
  polyLinePoints = referencedPoints;
}

void GeomPolyline::Translate ( double xShift, double yShift )
{
  Point pointShift( xShift, yShift );
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = referencedPoints[i] + pointShift;
}

void GeomPolyline::Rotate( double alpha )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = Point( referencedPoints[i].GetX() * cos(alpha) - referencedPoints[i].GetY() * sin(alpha),
                                 referencedPoints[i].GetX() * sin(alpha) + referencedPoints[i].GetY() * cos(alpha) );
}

void GeomPolyline::Scale( double xScaling, double yScaling )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i] = Point( referencedPoints[i].GetX() * xScaling, referencedPoints[i].GetY() * yScaling );
}

double GeomPolyline::DistanceToPoint ( Point point ) const
{
  std::vector<Point> polylinePoints;
  //GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
  return Curve::DistancePointToCurve( point, polylinePoints );
}
bool GeomPolyline::IsValid() const
{
  return referencedPoints.size();
}
}
