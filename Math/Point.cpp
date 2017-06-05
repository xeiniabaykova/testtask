#include "Point.h"
#include "Vector.h"
#include "CommonConstantsMath.h"
namespace Math {
//-----------------------------------------------------------------------------
/**
  Коструктор Point.
*/
//---
Point::Point( double x, double y):
  x(x),
  y(y)
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
	x += xShift;
	y += yShift;
}


//----------------------------------------------------------------------------
/**
  Повернуть на угол alpha относительно начала координат.
*/
//---
void Point::Rotate( double alpha )
{
  double cosAlpha = cos( alpha );
  double sinAlpha = sin( alpha );
  double oldx = x;
  double oldy = y;
  x = oldx * cosAlpha - oldy * sinAlpha;
  y = oldx * sinAlpha + oldy * cosAlpha;
}


//----------------------------------------------------------------------------
/**
  Маштабировать по оси х на xScaling, по оси y на yScaling.
*/
//---
void Point::Scale(  double xScaling, double yScaling  )
{
  x *= xScaling;
  y *= yScaling;
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
  return Point( x * param, y * param);
}


//-----------------------------------------------------------------------------
/**
  Получить разность точек ( по определению, это вектор).
*/
//---
Vector Point::operator - (Point point) const
{
  return Vector( x - point.GetX(), y - point.GetY() );
}


//-----------------------------------------------------------------------------
/**
  Сложить две точки.
*/
//---
Point Point::operator + ( Point point ) const
{
  return Point( x + point.GetX(), y + point.GetY());
}


//-----------------------------------------------------------------------------
/**
  Сложить ветор и точку. По определению, это точка.
*/
//---
Point Point::operator + ( Vector vector ) const
{
  return Point( x + vector.GetX(), y + vector.GetY() );
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
