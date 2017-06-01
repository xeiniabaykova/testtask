#include "Point.h"
#include "Vector.h"
#include "CommonConstantsMath.h"
namespace Math {
//-----------------------------------------------------------------------------
/**
  \ru Коструктор Point
*/
//-----------------------------------------------------------------------------
Point::Point( double x, double y):
  x(x),
  y(y)
{
}

Point::Point(const Point& point)
{
	x = point.GetX();
	y = point.GetY();
}
Point::Point(const Vector& vector)
{
  x = vector.GetX();
  y = vector.GetY();
}

void Point::Translate(  double xShift, double yShift )
{
	x += xShift;
	y += yShift;
}

void Point::Rotate( double alpha )
{
  double cosAlpha = cos( alpha );
  double sinAlpha = sin( alpha );
  x = x * cosAlpha - y * sinAlpha;
  y = x * sinAlpha + y * cosAlpha;
}

void Point::Scale(  double xScaling, double yScaling  )
{
  x *= xScaling;
  y *= yScaling;
}

double Point::GetX() const
{
	return x;
}
double Point::GetY() const
{
	return y;
}

Point Point::operator * (double param) const
{
  return Point( x * param, y * param);
}

Vector Point::operator - (Point point) const
{
  return Vector( x - point.GetX(), y - point.GetY() );
}

Point Point::operator + ( Point point ) const
{
  return Point( x + point.GetX(), y + point.GetY());
}

Point Point::operator + ( Vector vector ) const
{
  return Point( x + vector.GetX(), y + vector.GetY() );
}

Point Point::operator = ( Point point )
{
  x = point.GetX();
  y = point.GetY();
  return *this;
}

bool Point::IsValid() const
{
  return !( std::isnan(x) || std::isnan(y) );
}

double Distance( Point point1, Point point2 )
{
  return sqrt( (point1 - point2).GetX() * (point1 - point2).GetX() +
               (point1 - point2).GetY() * (point1 - point2).GetY() );
}

bool IsEqual( Point point1, Point point2 )
{
  return( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL );
}

}
