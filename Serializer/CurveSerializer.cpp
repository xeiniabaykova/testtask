#include "CurveSerializer.h"

#include <cinttypes>
#include <cstdio>

namespace Serializer {

//-----------------------------------------------------------------------------
/**
  Чтение точки из потока.
*/
//---

Math::Point CurveSerializer::ReadPoint( std::istream& input ) const
{
  double x = ReadDouble(input);
  double y = ReadDouble(input);
  return Math::Point( x,y );
}



//-----------------------------------------------------------------------------
/**
  Запись точки в поток.
*/
//---
void CurveSerializer::WritePoint( std::ostream& output, const Math::Point& point ) const
{
  WriteDouble ( output, point.GetX() );
  WriteDouble ( output, point.GetY() );
}


//-----------------------------------------------------------------------------
/**
  Чтение числа с плавающей запятой из потока.
*/
//---
double CurveSerializer::ReadDouble( std::istream& input ) const
{
  double value;
  input.read( (char*) &value, sizeof value );
  return value;
}


//-----------------------------------------------------------------------------
/**
  Чтение целочисленного значения из потока.
*/
//---
int CurveSerializer::ReadInt( std::istream& input ) const
{
  int value;
  input.read( (char*)&value, sizeof value );
  return value;
}


//-----------------------------------------------------------------------------
/**
  Запись числа с плавающей запятой в поток.
*/
//---
void CurveSerializer::WriteDouble( std::ostream& output, double value ) const
{
  output.write( (char*)&value, sizeof value );
}


//-----------------------------------------------------------------------------
/**
  Запись целочисленного значения в поток.
*/
//---
void CurveSerializer::WriteInt( std::ostream& output, int value ) const
{
  output.write( (char*)&value, sizeof(value) );
}
}
