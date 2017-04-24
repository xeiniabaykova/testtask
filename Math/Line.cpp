#include "Line.h"


//-----------------------------------------------------------------------------
/**
  \ru Коструктор Line Записывается начальная точка и вектор направления
*/
//-----------------------------------------------------------------------------
Line::Line( Point startPoint, Point endPoint ):
  startPoint( startPoint ),
  direction( endPoint.GetX() - startPoint.GetX(), endPoint.GetY() - startPoint.GetY() )
{}

//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для прямой: [0, 1]
*/
//-----------------------------------------------------------------------------
Range Line::GetRange() const
{
  return Range( 0.0, 1.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Line::GetPoint( double t ) const
{
  return Point( startPoint.GetX() + direction.GetX() * t, startPoint.GetY() + direction.GetY() * t );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Point Line::GetDerivativePoint( double t ) const
{
  (void)t;
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Point Line::Get2DerivativePoint( double t ) const
{
  (void)t;
  return Point ( 0.0, 0.0 );
}
