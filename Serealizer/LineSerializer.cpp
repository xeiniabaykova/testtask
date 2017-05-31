#include "LineSerializer.h"


std::string LineSerializer::GetHeaderName()
{
  return Math::Line( Math::Point(), Math::Point() ).GetName();
}

std::unique_ptr<Math::GeometricPrimitive> LineSerializer::Read( std::istream& theInput )
{
  Math::Point aPoint1;
  Math::Point aPoint2;
  aPoint1 = ReadPoint( theInput );
  aPoint2 = ReadPoint( theInput );
  return std::make_unique<Math::Line>( aPoint1, aPoint2 );
}

void LineSerializer::Write ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve )
{
  WritePoint( theOutput, dynamic_cast<const Math::Line&> (theCurve).GetPoint(0) );
  WritePoint( theOutput, dynamic_cast<const Math::Line&> (theCurve).GetPoint(1) );
}
