#include "LineSerializer.h"
#include "Math/Line.h"

std::string LineSerializer::GetHeaderName() {
  return Line( Point(), Point() ).GetName();
}

std::unique_ptr<GeometricPrimitive> LineSerializer::Read( std::istream& theInput ) {
  Point aPoint1;
  Point aPoint2;
  aPoint1 = ReadPoint( theInput );
  aPoint2 = ReadPoint( theInput );
  return std::make_unique<Line> (aPoint1, aPoint2);
}

void LineSerializer::Write ( std::ostream& theOutput, const GeometricPrimitive& theCurve ) {
  WritePoint( theOutput, dynamic_cast<const Line&> (theCurve).GetPoint(0) );
  WritePoint( theOutput, dynamic_cast<const Line&> (theCurve).GetPoint(1) );
}
