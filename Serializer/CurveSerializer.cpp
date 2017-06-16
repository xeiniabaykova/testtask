#include "CurveSerializer.h"

#include <cinttypes>


namespace Serializer {

//-----------------------------------------------------------------------------
// Прочитать точку из потока.Точка читается покоординатно: х, y.
//---

Math::Point CurveSerializer::ReadPoint( std::istream& input ) const
{
  double x = ReadDouble( input );
  double y = ReadDouble( input );
  return Math::Point( x,y );
}



//-----------------------------------------------------------------------------
//  Запись точки в поток. Точка записывается покоординатно: х, y.
// ---
void CurveSerializer::WritePoint( std::ostream& output, const Math::Point& point ) const
{
  WriteDouble ( output, point.GetX() );
  WriteDouble ( output, point.GetY() );
}


//-----------------------------------------------------------------------------
//  Прочитать число с плавающей запятой из потока.
// ---
double CurveSerializer::ReadDouble( std::istream& input ) const
{
  double value;
  input.read( (char*) &value, sizeof value );
  return value;
}


//-----------------------------------------------------------------------------
//  Прочитать целочисленное значение из потока.
// ---
size_t CurveSerializer::ReadSize_t( std::istream& input ) const
{
  size_t value;
  input.read( (char*)&value, sizeof value );
  return value;
}


//-----------------------------------------------------------------------------
//  Записать число с плавающей запятой в поток.
// ---
void CurveSerializer::WriteDouble( std::ostream& output, double value ) const
{
  output.write( (char*)&value, sizeof value );
}


//-----------------------------------------------------------------------------
//  Записать целочисленное значения в поток.
// ---
void CurveSerializer::WriteSize_t( std::ostream& output, size_t value ) const
{
  output.write( (char*)&value, sizeof(value) );
}
}
