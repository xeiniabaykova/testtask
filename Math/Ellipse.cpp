#include "Ellipse.h"
#include <vector>
#include <cmath>
#include "Editor/Polyline.h"


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
}


//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetPoint( double t ) const
{
   Point point( r1 * sin(t), r2 * cos(t) );
   Point transformPoint( center.GetX() +point.GetX() * cos(alpha) + point.GetY() * sin(alpha),
                        center.GetY() + point.GetX() * sin(alpha) + point.GetY() * cos(alpha) );
   return transformPoint;
}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для эллипса
*/
//-----------------------------------------------------------------------------


Range Ellipse::GetRange() const
{
  return Range( 0.0, 2.0 * 3.14 );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную элипса по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetDerivativePoint( double t ) const
{
  Point point( r1 * cos(t), -r2 * sin(t) );

  return point;

}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую эллипса по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::Get2DerivativePoint( double t ) const
{
  Point point( -r1 * sin(t), -r2 * cos(t) );
  return point;
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для эллипса - используется общий алгоритм
*/
//-----------------------------------------------------------------------------
void Ellipse::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  Polyline polyline( this, accuracy );
  polyLinePoints = polyline.GetPoints();
}


