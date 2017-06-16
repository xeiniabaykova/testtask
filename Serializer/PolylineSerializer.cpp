#include "PolylineSerializer.h"

namespace Serializer {
//-----------------------------------------------------------------------------
//  Получение заголовка примитва Nurbs - кривая.
// ---
std::string PolylineSerializer::GetHeaderName()
{
  return Math::GeomPolyline().GetName();
}


//-----------------------------------------------------------------------------
// Чтение полилинии из потока. Данные читаются в следующем порядке: количество точек, точки (Point).
// ---
std::unique_ptr<Math::Curve> PolylineSerializer::Read( std::istream& theInput )
{
  size_t numWeights;
  size_t numPoles;
  size_t degree;
  bool isClosed;
  numPoles = ReadSize_t( theInput );
  // Чтение опорных точек.
  std::vector<Math::Point> controlPoints;
  for ( size_t i = 0; i < numPoles; i++ )
  {
    controlPoints.push_back ( ReadPoint(theInput) );
  }
  return std::make_unique<Math::GeomPolyline>( controlPoints );

}


//-----------------------------------------------------------------------------
// Данные записываются в следующем порядке: количество точек, точки (Point).
// ---
 void PolylineSerializer::Write( std::ostream& theOutput, const Math::Curve& theCurve )
 {
  const Math::GeomPolyline* nurbsCurve =  static_cast<const Math::GeomPolyline*>( &theCurve );
  if ( nurbsCurve != nullptr )
  {
    const auto points = nurbsCurve.GetReferensedPoints();
    WriteSize_t( theOutput, points.size() );
    for ( size_t i = 0; i < points.size(); i++ )
    {
      WritePoint ( theOutput, points[i] );
    }
  }

}
}
