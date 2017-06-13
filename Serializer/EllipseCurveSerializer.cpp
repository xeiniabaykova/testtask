#include "EllipseCurveSerializer.h"

namespace Serializer {

//-----------------------------------------------------------------------------
/**
  Чтение целочисленного значения из потока.
*/
//---
std::string EllipseCurveSerializer::GetHeaderName()
{
  return Math::Ellipse().GetName();
}

std::unique_ptr<Math::Curve> EllipseCurveSerializer::Read( std::istream& theInput )
{
  Math::Point aCenter = ReadPoint( theInput );
  double aRadius1 = ReadDouble( theInput );
  double aRadius2 = ReadDouble( theInput );
  double alpha = ReadDouble( theInput );
  return std::make_unique<Math::Ellipse>( aCenter, aRadius1, aRadius2, alpha );
}

void EllipseCurveSerializer::Write( std::ostream& theOutput, const Math::Curve& theCurve )
{
  WritePoint( theOutput, dynamic_cast<const Math::Ellipse&>(theCurve).GetCenter() );
  WriteDouble( theOutput, dynamic_cast<const Math::Ellipse&>(theCurve).GetMajorRadius() );
  WriteDouble( theOutput, dynamic_cast<const Math::Ellipse&>(theCurve).GetMinorRadius() );
  WriteDouble( theOutput, dynamic_cast<const Math::Ellipse&>(theCurve).GetAlpha() );
}
}
