#include "CurveSerializer.h"

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

double CurveSerializer::ReadInt( std::istream& input ) const
{
  int value;
  input.read( (char*)&value, sizeof value );
  return value;
}
void CurveSerializer::WriteDouble( std::ostream& output, double value ) const
{
  output.write( (char*)&value, sizeof value );
}

void CurveSerializer::WriteInt( std::ostream& output, int value ) const
{
  output.write( (char*)&value, sizeof value );
}
