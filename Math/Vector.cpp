#include "Vector.h"
#include <cmath>

namespace Math {

//----------------------------------------------------------------------------
/**
	Коструткор по двум значениям.
*/
//---
Vector::Vector( double x1, double x2 ):
  x( x1 ),
  y( x2 )
{
}


//----------------------------------------------------------------------------
/**
	Коструткор копии.
*/
//---
Vector::Vector( const Vector& vector )
{
  x = vector.x;
  y = vector.y;
}


//----------------------------------------------------------------------------
/**
	Получить координату x.
*/
//---
double Vector::GetX() const
{
  return x;
}


//----------------------------------------------------------------------------
/**
	Получить координату y.
*/
//---
double Vector::GetY() const
{
  return y;
}


//----------------------------------------------------------------------------
/**
	Сдвинуть по осям x, y на xShift, yShift .
*/
//---
void Vector::Translate ( double xShift, double yShift )
{
  x = x + xShift;
  y = y + yShift;

}


//----------------------------------------------------------------------------
/**
	Повернуть на угол alpha относительно начала координат.
*/
//---
void Vector::Rotate( double alpha )
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
void Vector::Scale( double xScaling, double yScaling )
{
  x *= xScaling;
  y *= yScaling;

}


//----------------------------------------------------------------------------
/**
	Домножить вектор на скаляр.
*/
//---
Vector Vector::operator * ( double param  ) const
{
  return Vector( x * param, y * param );
}


//----------------------------------------------------------------------------
/**
	Получить разность векторов.
*/
//---
Vector Vector::operator - ( Vector point ) const
{
  return Vector( x - point.GetX(), y -point.GetY() );
}


//----------------------------------------------------------------------------
/**
	Получить сумму векторов.
*/
//---
Vector Vector::operator + ( Vector point ) const
{
  return Vector( x + point.GetX(), y + point.GetY() );
}


//----------------------------------------------------------------------------
/**
	Присвоить один вектор другому.
*/
//---
Vector Vector::operator = ( Vector point )
{
  x = point.GetX();
  y = point.GetY();
  return *this;
}


//----------------------------------------------------------------------------
/**
	Получить скалярное произведение векторов по определению.
*/
//---
double Vector::operator * ( Vector vector ) const
{
  return x * vector.x + y * vector.y;
}


//----------------------------------------------------------------------------
/**
	Проверить вектор на правильность.
*/
//---
bool Vector::IsValid() const
{
  return true;
}
}
