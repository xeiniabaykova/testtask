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
	Если вектор или одно из значений не является валидным, вектор не изменяется.
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
	Если вектор или одно из значений не является валидным, вектор не изменяется.
*/
//---
void Vector::Rotate( double alpha )
{
	if ( this->IsValid() && !std::isnan(alpha) )
	{
		double cosAlpha = cos(alpha);
		double sinAlpha = sin(alpha);
		double oldx = x;
		double oldy = y;
		x = oldx * cosAlpha - oldy * sinAlpha;
		y = oldx * sinAlpha + oldy * cosAlpha;
	}
}


//----------------------------------------------------------------------------
/**
	Маштабировать по оси х на xScaling, по оси y на yScaling.
	Если вектор или одно из значений не является валидным, вектор не изменяется.
*/
//---
void Vector::Scale( double xScaling, double yScaling )
{
	if ( this->IsValid() && !std::isnan(xScaling) && !std::isnan(yScaling) )
	{
		x *= xScaling;
		y *= yScaling;
	}

}


//----------------------------------------------------------------------------
/**
	Домножить вектор на скаляр.
	Если одно из значений не является валидным, возвращается NAN.
*/
//---
Vector Vector::operator * ( double param  ) const
{
	if ( this->IsValid() && !std::isnan(param) )
	{
		return Vector(x * param, y * param);
	}
}


//----------------------------------------------------------------------------
/**
	Получить разность векторов.
	Если одно из значений не является валидным, возвращается NAN.
*/
//---
Vector Vector::operator - ( Vector vector ) const
{
	if (vector.IsValid() && this->IsValid())
	{
		return Vector(x - vector.GetX(), y - vector.GetY());
	}
	else
		return  Vector(NAN, NAN);;
}


//----------------------------------------------------------------------------
/**
	Получить сумму векторов.
	Если одно из значений не является валидным, возвращается NAN.
*/
//---
Vector Vector::operator + ( Vector vector ) const
{
	if (vector.IsValid() && this->IsValid())
	{
		return Vector(x + vector.GetX(), y + vector.GetY());
	}
	else
		return Vector(NAN, NAN);
}


//----------------------------------------------------------------------------
/**
	Присвоить один вектор другому.
	Если одно из значений не является валидным, возвращается NAN.
*/
//---
Vector Vector::operator = ( Vector vector )
{
	if (vector.IsValid() && this->IsValid())
	{
		x = vector.GetX();
		y = vector.GetY();
		return *this;
	}
	else
		return Vector(NAN, NAN);
}


//----------------------------------------------------------------------------
/**
	Получить скалярное произведение векторов по определению.
	Если одно из значений не является валидным, возвращается NAN.
*/
//---
double Vector::operator * ( Vector vector ) const
{
	if ( vector.IsValid() && this->IsValid() )
		return x * vector.x + y * vector.y;
  else
    return NAN;
}


//----------------------------------------------------------------------------
/**
  Проверить вектор на правильность. Вектор считается верным, если ни одно значение не является nan.
*/
//---
bool Vector::IsValid() const
{
  return !( std::isnan(x) || std::isnan(y) );
}

//----------------------------------------------------------------------------
/**
  Получить векторное произведение векторов по определению.
*/
//---
double Vector::VectorMult( Vector vector ) const
{
  if ( vector.IsValid() && this->IsValid() )
    return x * vector.y - y * vector.x;
  else
    return NAN;

}
}
