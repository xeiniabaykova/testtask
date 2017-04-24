#include "Ellipse.h"
#include <vector>
#include <cmath>


//-----------------------------------------------------------------------------
/**
  \ru Коструктор Ellipse. Вычисляются два радиуса и записывается центральная точка
*/
//-----------------------------------------------------------------------------
Ellipse::Ellipse( Point center, double r1, double r2 ):
  center(center),
  r1(r1),
  r2(r2){}

//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetPoint( double t ) const
{
  return Point ( center.GetX() + r1 * sin(t), center.GetY() + r2 * cos(t) );
}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для эллипса
*/
//-----------------------------------------------------------------------------
Range Ellipse::GetRange() const
{
  return Range ( 0.0, 2.0 * 3.14 );
}