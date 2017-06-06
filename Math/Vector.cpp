#include "Vector.h"

namespace Math {

//----------------------------------------------------------------------------
/**
	���������� �� ���� ���������.
*/
//---
Vector::Vector( double x1, double x2 ):
  x( x1 ),
  y( x2 )
{
}


//----------------------------------------------------------------------------
/**
	���������� �����.
*/
//---
Vector::Vector( const Vector& vector )
{
  x = vector.x;
  y = vector.y;
}


//----------------------------------------------------------------------------
/**
	�������� ���������� x.
*/
//---
double Vector::GetX() const
{
  return x;
}


//----------------------------------------------------------------------------
/**
	�������� ���������� y.
*/
//---
double Vector::GetY() const
{
  return y;
}


//----------------------------------------------------------------------------
/**
	�������� �� ���� x, y �� xShift, yShift .
*/
//---
void Vector::Translate ( double xShift, double yShift )
{
  x = x + xShift;
  y = y + yShift;

}


//----------------------------------------------------------------------------
/**
	��������� �� ���� alpha ������������ ������ ���������.
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
	������������� �� ��� � �� xScaling, �� ��� y �� yScaling.
*/
//---
void Vector::Scale( double xScaling, double yScaling )
{
  x *= xScaling;
  y *= yScaling;

}


//----------------------------------------------------------------------------
/**
	��������� ������ �� ������.
*/
//---
Vector Vector::operator * ( double param  ) const
{
  return Vector( x * param, y * param );
}


//----------------------------------------------------------------------------
/**
	�������� �������� ��������.
*/
//---
Vector Vector::operator - ( Vector point ) const
{
  return Vector( x - point.GetX(), y -point.GetY() );
}


//----------------------------------------------------------------------------
/**
	�������� ����� ��������.
*/
//---
Vector Vector::operator + ( Vector point ) const
{
  return Vector( x + point.GetX(), y + point.GetY() );
}


//----------------------------------------------------------------------------
/**
	��������� ���� ������ �������.
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
	�������� ��������� ������������ �������� �� �����������.
*/
//---
double Vector::operator * ( Vector vector ) const
{
  return x * vector.x + y * vector.y;
}


//----------------------------------------------------------------------------
/**
	��������� ������ �� ������������.
*/
//---
bool Vector::IsValid() const
{
  return true;
}
}
