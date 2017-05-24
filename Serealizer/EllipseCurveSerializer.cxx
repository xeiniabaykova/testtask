#include "EllipseCurveSerializer.hxx"

std::string EllipseCurveSerializer::GetHeaderName()
{
  return Ellipse().GetName();
}

std::unique_ptr<GeometricPrimitive> EllipseCurveSerializer::Read( std::istream& theInput )
{
  Point aCenter = ReadPoint( theInput );
  double aRadius1 = ReadDouble( theInput );
  double aRadius2 = ReadDouble( theInput );
  double alpha = ReadDouble( theInput );
  return std::make_unique<Ellipse>( aCenter, aRadius1, aRadius2, alpha );
}

void EllipseCurveSerializer::Write( std::ostream& theOutput, const GeometricPrimitive& theCurve )
{
  WritePoint( theOutput, dynamic_cast<const Ellipse&>(theCurve).GetCenter() );
  WriteDouble( theOutput, dynamic_cast<const Ellipse&>(theCurve).GetMajorRadius() );
  WriteDouble( theOutput, dynamic_cast<const Ellipse&>(theCurve).GetMinorRadius() );
  WriteDouble( theOutput, dynamic_cast<const Ellipse&>(theCurve).GetAlpha() );
}

