#include "CurveSerializer.h"
#include <cinttypes>
#include <cstdio>

Point CurveSerializer::ReadPoint(std::istream& input) const
{
  Point point( ReadDouble(input), ReadDouble(input) );
  return point;
}

void CurveSerializer::WritePoint( std::ostream& output, const Point& point ) const
{
  WriteDouble ( output, point.GetX() );
  WriteDouble ( output, point.GetY() );
}

double CurveSerializer::ReadDouble( std::istream& input ) const
{
  double value;
  input.read( (char*) &value, sizeof value );
  return value;
}

int CurveSerializer::ReadInt( std::istream& input ) const
{

#if ENVIRONMENT32
  std::int32_t value;
#else
  std::int64_t value;
#endif
  input.read( (char*)&value, sizeof value );
  return value;
}
void CurveSerializer::WriteDouble( std::ostream& output, double value ) const
{
  output.write( (char*)&value, sizeof value );
}

void CurveSerializer::WriteInt( std::ostream& output, int value ) const
{
#if ENVIRONMENT32
  std::int32_t size;
#else
  std::int64_t size;
#endif
  output.write( (char*)&value, sizeof(size) );
}
