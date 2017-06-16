#include <cmath>
#include <limits>
#include "Point.h"

#include "CommonConstantsMath.h"


namespace Math {
//-----------------------------------------------------------------------------
// Коструктор Point.
// ---
Point::Point( double theX, double theY):
  GeometricPrimitive(),
  x                 ( theX ),
  y                 ( theY )
{
}


//----------------------------------------------------------------------------
//  Коструктор копии.
// ---
Point::Point(const Point& point):
  GeometricPrimitive(),
  x(0),
  y(0)
{
	x = point.GetX();
	y = point.GetY();
}

//----------------------------------------------------------------------------
//  Коструктор создания точки из вектора.
// ---
Point::Point(const Vector& vector):
  GeometricPrimitive(),
  x(0),
  y(0)
{
  x = vector.GetX();
  y = vector.GetY();
}

//----------------------------------------------------------------------------
//  Сдвинуть по осям x, y.
// ---
void Point::Translate( double xShift, double yShift )
{
  if ( IsValid() )
  {
    x += xShift;
    y += yShift;
  }
}


//----------------------------------------------------------------------------
//  Повернуть на угол alpha относительно начала координат.
// ---
void Point::Rotate( double alpha )
{
  if ( IsValid() )
  {
    const double cosAlpha = cos( alpha );
    const double sinAlpha = sin( alpha );
    const double oldx = x;
    const double oldy = y;
    x = oldx * cosAlpha - oldy * sinAlpha;
    y = oldx * sinAlpha + oldy * cosAlpha;
  }
}


//----------------------------------------------------------------------------
//  Маштабировать по оси х на xScaling, по оси y на yScaling.
// ---
void Point::Scale(  double xScaling, double yScaling  )
{
  if ( IsValid() )
  {
    x *= xScaling;
    y *= yScaling;
  }
}


//----------------------------------------------------------------------------
//  Получить координату x.
// ---
double Point::GetX() const
{
	return x;
}


//-----------------------------------------------------------------------------
// Получить координату y.
// ---
double Point::GetY() const
{
	return y;
}


//-----------------------------------------------------------------------------
// Умножить точку на скаляр.
// ---
Point Point::operator * (double param) const
{
  if ( IsValid() )
  {
    return Point( x * param, y * param);
  }
  else
    return Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
}


//-----------------------------------------------------------------------------
// Разделить точку на скаляр.
// ---
Point Point::operator / (double param) const
{
  if ( IsValid() && fabs(param) > CommonConstantsMath::NULL_TOL  )
  {
    return Point( x / param, y / param);
  }
  else
    return Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
}


//-----------------------------------------------------------------------------
// Получить разность точек ( по определению, это вектор).
// ---
Vector Point::operator - (Point point) const
{
  if ( IsValid() && point.IsValid() )
  {
    return Vector( x - point.GetX(), y - point.GetY() );
  }
  else
    return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
}


//-----------------------------------------------------------------------------
//  Сложить две точки.
// ---
Point Point::operator + ( Point point ) const
{
  if ( IsValid() && point.IsValid() )
  {
    return Point( x + point.GetX(), y + point.GetY());
  }
  else
    return Point( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
//  Сложить ветор и точку. По определению, это точка.
// ---
Point Point::operator + ( Vector vector ) const
{
  if ( IsValid() && vector.IsValid() )
  {
    return Point( x + vector.GetX(), y + vector.GetY() );
  }
  else
    return Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
//  Прировнять одну точку к другой точке.
// ---
Point Point::operator = ( Point point )
{
  x = point.GetX();
  y = point.GetY();
  return *this;
}

//-----------------------------------------------------------------------------
//  Присвоить к текущей точке другую точку.
// ---
Point Point::operator += ( Point point )
{
  x += point.x;
  y += point.y;
  return *this;
}



//-----------------------------------------------------------------------------
//  Проверить точки точки на правильность.
// ---
bool Point::IsValid() const
{
  return !( std::isinf(x) || std::isinf(y) );
}


//-----------------------------------------------------------------------------
//  Вернуть имя, используемое при записи точки в файл.
// ---
std::string Point::GetName() const
{
  return "Vector";
}

//-----------------------------------------------------------------------------
// Найти расстояние между точками.
// ---
double Distance( Point point1, Point point2 )
{
  return sqrt( (point1 - point2).GetX() * (point1 - point2).GetX() +
               (point1 - point2).GetY() * (point1 - point2).GetY() );
}


//-----------------------------------------------------------------------------
//  Проверить точки на эквивалентность с точностью CommonConstantsMath::NULL_TOL.
// ---
bool IsEqual( Point point1, Point point2 )
{
  return( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL );
}
}


