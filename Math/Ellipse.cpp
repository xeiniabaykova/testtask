#include "Ellipse.h"
#include <vector>
#include <cmath>
#include "Editor/Polyline.h"
#include "Editor/CommonConstants.h"


//-----------------------------------------------------------------------------
/**
  \ru Коструктор Ellipse. Вычисляются два радиуса и записывается центральная точка
*/
//-----------------------------------------------------------------------------
Ellipse::Ellipse( Point center, double r1, double r2,double alpha ):
  center( center ),
  r1    ( r1     ),
  r2    ( r2     ),
  alpha ( alpha  )
{
  std::vector<Point> points;
  points.push_back( center );
  SetReferensedPoints( points );
}


Ellipse::Ellipse ( const std::vector<Point>& points )
{  
  center = points[0];
  Point pointV( points[1] );

  Point pointX( center.GetX(), points[1].GetY() );

  double x = pointV.GetX() - center.GetX();
  double y = pointV.GetY() - center.GetY();
  double axisA = sqrt( x * x + y * y );

  if ( abs(x) < 0.001 && y <= 0 )
    alpha = -Pi/2.0;

  if ( abs(x) < 0.001 && y > 0  )
    alpha = Pi/2.0;

  if ( x > 0 && y >= 0 )
    alpha = atan( y / x );

  if ( x < 0 && y >= 0 )
    alpha = Pi + atan( y / x );

  if ( x < 0 && y <= 0 )
    alpha = Pi + atan( y / x );

  if ( x > 0 && y <= 0 )
    alpha = - atan( y / x );

  r1 = axisA;
  Point newCoordPoint( points[2].GetX() - center.GetX(), points[2].GetY() - center.GetY() );
  Point point2( newCoordPoint.GetX() * cos(alpha) + newCoordPoint.GetY() * sin(alpha),
                      - newCoordPoint.GetX() * sin(alpha) + newCoordPoint.GetY() * cos(alpha) );

  double axisB = (sqrt( (point2.GetY()) * (point2.GetY()) /
      ( 1 - ( point2.GetX()) * ( point2.GetX()) / (r1 * r1))) );

  r2 = axisB;

}

//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetPoint( double t ) const
{

   Point point( r1 * cos(t), r2 * sin(t) );
   Point transformPoint( center.GetX() + point.GetX() * cos(alpha) - point.GetY() * sin(alpha),
                        center.GetY() + point.GetX() * sin(alpha)   + point.GetY() * cos(alpha) );
   return transformPoint;
}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для эллипса
*/
//-----------------------------------------------------------------------------


Range Ellipse::GetRange() const
{
  return Range( 0.0, 2.0 * Pi );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную элипса по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetDerivativePoint( double t ) const
{
  Point point( r1 * -sin(t), r2 * cos(t) );

  return point;

}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую эллипса по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::Get2DerivativePoint( double t ) const
{
  Point point( -r1 * cos(t), -r2 * sin(t) );
  return point;
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для эллипса - используется общий алгоритм
*/
//-----------------------------------------------------------------------------
void Ellipse::GetAsPolyLine( std::vector<Point>& polyLinePoints, double accuracy ) const
{
  Polyline polyline( this, accuracy );
  polyLinePoints = polyline.GetPoints();
}

double Ellipse::DistancePointToCurve( Point point ) const
{
  double accuracy = 0.01;
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, accuracy );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}

void Ellipse::Translation( const std::vector<std::vector<double>>& )
{
  return;
}

void Ellipse::Rotation( const std::vector<std::vector<double>>& )
{
  return;
}

void Ellipse::Dilatation( const std::vector<std::vector<double>>& )
{
  return;
}
