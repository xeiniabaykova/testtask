#include "Point.h"

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

void Point::Translate(  double xShift, double yShift )
{
	x += xShift;
	y += yShift;
}

void Point::Rotate( double )
{
  return;
}

void Point::Scale(  double, double  )
{
  return;
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
  return Point(x * param, y * param);
}
Point Point::operator - (Point point) const
{
  return Point( x - point.GetX(), y -point.GetY() );
}
Point Point::operator + (Point point) const
{
  return Point(x + point.GetX(), y + point.GetY());
}
Point Point::operator = ( Point point )
{
  x = point.GetX();
  y = point.GetY();
  return *this;
}
