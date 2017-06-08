#include <cmath>
#include "Point.h"
#include "CommonConstantsMath.h"


namespace Math {
//-----------------------------------------------------------------------------
/**
  Коструктор Point.
*/
//---
Point::Point( double theX, double theY):
  x( theX ),
  y( theY )
{
}


//----------------------------------------------------------------------------
/**
  Коструктор копии.
*/
//---
Point::Point(const Point& point)
{
	x = point.GetX();
	y = point.GetY();
}

//----------------------------------------------------------------------------
/**
  Коструктор создания точки из вектора.
*/
//---
Point::Point(const Vector& vector)
{
  x = vector.GetX();
  y = vector.GetY();
}

//----------------------------------------------------------------------------
/**
  Сдвинуть по осям x, y.
*/
//---
void Point::Translate(  double xShift, double yShift )
{
  if ( IsValid() )
  {
    x += xShift;
    y += yShift;
  }
}


//----------------------------------------------------------------------------
/**
  Повернуть на угол alpha относительно начала координат.
*/
//---
void Point::Rotate( double alpha )
{
  if ( IsValid() )
  {
    double cosAlpha = cos( alpha );
    double sinAlpha = sin( alpha );
    double oldx = x;
    double oldy = y;
    x = oldx * cosAlpha - oldy * sinAlpha;
    y = oldx * sinAlpha + oldy * cosAlpha;
  }
}


//----------------------------------------------------------------------------
/**
  Маштабировать по оси х на xScaling, по оси y на yScaling.
*/
//---
void Point::Scale(  double xScaling, double yScaling  )
{
  if ( IsValid() )
  {
    x *= xScaling;
    y *= yScaling;
  }
}


//----------------------------------------------------------------------------
/**
  Получить координату x.
*/
//---
double Point::GetX() const
{
	return x;
}


//-----------------------------------------------------------------------------
/**
  Получить координату y.
*/
//---
double Point::GetY() const
{
	return y;
}


//-----------------------------------------------------------------------------
/**
  Умножить точку на скаляр.
*/
//---
Point Point::operator * (double param) const
{
  if ( IsValid() )
  {
    return Point( x * param, y * param);
  } else
    return Point( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Получить разность точек ( по определению, это вектор).
*/
//---
Vector Point::operator - (Point point) const
{
  if ( IsValid() && point.IsValid() )
  {
    return Vector( x - point.GetX(), y - point.GetY() );
  } else
    return Vector( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Сложить две точки.
*/
//---
Point Point::operator + ( Point point ) const
{
  if ( IsValid() && point.IsValid() )
  {
    return Point( x + point.GetX(), y + point.GetY());
  } else
    return Point( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Сложить ветор и точку. По определению, это точка.
*/
//---
Point Point::operator + ( Vector vector ) const
{
  if ( IsValid() && vector.IsValid() )
  {
    return Point( x + vector.GetX(), y + vector.GetY() );
  } else
    return Point( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Прировнять одну точку к другой точке.
*/
//---
Point Point::operator = ( Point point )
{
  x = point.GetX();
  y = point.GetY();
  return *this;
}


//-----------------------------------------------------------------------------
/**
  Проверить точки точки на правильность.
*/
//---
bool Point::IsValid() const
{
  return !( std::isnan(x) || std::isnan(y) );
}


//-----------------------------------------------------------------------------
/**
  Найти расстояние между точками.
*/
//---
double Distance( Point point1, Point point2 )
{
  return sqrt( (point1 - point2).GetX() * (point1 - point2).GetX() +
               (point1 - point2).GetY() * (point1 - point2).GetY() );
}


//-----------------------------------------------------------------------------
/**
  Проверить точки на эквивалентность с точностью CommonConstantsMath::NULL_TOL.
*/
//---
bool IsEqual( Point point1, Point point2 )
{
  return( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL );
}

}
