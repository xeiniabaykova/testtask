#include "Vector.h"
#include <cmath>
#include <limits>
#include "CommonConstantsMath.h"

namespace Math {

//----------------------------------------------------------------------------
// Коструткор по двум значениям.
// ---
Vector::Vector( double x1, double x2 ):
  GeometricPrimitive(),
  x( x1 ),
  y( x2 )
{
}


//----------------------------------------------------------------------------
// Коструткор копии.
// ---
Vector::Vector( const Vector& vector ):
  GeometricPrimitive(),
  x(0),
  y(0)
{
  x = vector.x;
  y = vector.y;
}


//----------------------------------------------------------------------------
//	Получить координату x.
// ---
double Vector::GetX() const
{
  return x;
}


//----------------------------------------------------------------------------
//	Получить координату y.
// ---
double Vector::GetY() const
{
  return y;
}


//----------------------------------------------------------------------------
//	Сдвинуть по осям x, y на xShift, yShift .
//	Если вектор или одно из значений не является валидным, вектор не изменяется.
// ---
void Vector::Translate ( double xShift, double yShift )
{
  x = x + xShift;
  y = y + yShift;
}


//----------------------------------------------------------------------------
//	Повернуть на угол alpha относительно начала координат.
//	Если вектор или одно из значений не является валидным, вектор не изменяется.
// ---
void Vector::Rotate( double alpha )
{
	if ( this->IsValid() && !std::isinf(alpha) )
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
//	Маштабировать по оси х на xScaling, по оси y на yScaling.
//	Если вектор или одно из значений не является валидным, вектор не изменяется.
// ---
void Vector::Scale( double xScaling, double yScaling )
{
	if ( this->IsValid() && !std::isinf(xScaling) && !std::isinf(yScaling) )
	{
		x *= xScaling;
		y *= yScaling;
	}

}


//----------------------------------------------------------------------------
//	Домножить вектор на скаляр.
//	Если одно из значений не является валидным, возвращается вектор из inf.
// ---
Vector Vector::operator * ( double param  ) const
{
	if ( this->IsValid() && !std::isinf(param) )
	{
		return Vector(x * param, y * param);
	}
	else
		return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//----------------------------------------------------------------------------
//	Получить разность векторов.
//	Если одно из значений не является валидным, возвращается vector из inf.
// ---
Vector Vector::operator - ( Vector vector ) const
{
	if (vector.IsValid() && this->IsValid())
	{
		return Vector(x - vector.GetX(), y - vector.GetY());
	}
	else
		return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//----------------------------------------------------------------------------
//	Получить сумму векторов.
//	Если одно из значений не является валидным, возвращается вектор из inf.
// ---
Vector Vector::operator + ( Vector vector ) const
{
  if ( vector.IsValid() && this->IsValid() )
	{
    return Vector( x + vector.GetX(), y + vector.GetY() );
	}
	else
		return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//----------------------------------------------------------------------------
//	Присвоить один вектор другому.
//	Если одно из значений не является валидным, возвращается вектор из inf.
// ---
Vector Vector::operator = ( Vector vector )
{
  if ( vector.IsValid() && this->IsValid() )
	{
		x = vector.GetX();
		y = vector.GetY();
		return *this;
	}
	else
		return Vector(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
}


//----------------------------------------------------------------------------
//	Получить скалярное произведение векторов по определению.
//	Если одно из значений не является валидным, возвращается inf.
// ---
double Vector::operator * ( Vector vector ) const
{
	if ( vector.IsValid() && this->IsValid() )
		return x * vector.x + y * vector.y;
  else
    return std::numeric_limits<double>::infinity();
}


//----------------------------------------------------------------------------
//  Проверить вектор на правильность. Вектор считается верным, если ни одно значение не является inf.
// ---
bool Vector::IsValid() const
{
  return !( std::isinf(x) || std::isinf(y) );
}

//----------------------------------------------------------------------------
//  Получить векторное произведение векторов по определению.
// ---
double Vector::VectorMult( Vector vector ) const
{
  if ( vector.IsValid() && this->IsValid() )
    return x * vector.y - y * vector.x;
  else
    return std::numeric_limits<double>::infinity();

}


//----------------------------------------------------------------------------
//  Получить длину вектора по определению.
// ---
double Vector::Lenght() const
{
	return sqrt( x * x + y * y );
}


//----------------------------------------------------------------------------
//   Проверить вектора на параллельность.
// ---
bool Vector::IsCollinear( Vector vector, double accuracy ) const
{
  // векорное произведение
  return ( std::fabs( VectorMult(vector) ) < accuracy );

}
//----------------------------------------------------------------------------
/**
  Вернуть имя, используемое при записи точки в файл.
*/
//---
std::string Vector::GetName() const
{
  return "Vector";
}
}
