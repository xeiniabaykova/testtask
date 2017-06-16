#include "NURBSCurveSerializer.h"

namespace Serializer {
//-----------------------------------------------------------------------------
//  Получение заголовка примитва Nurbs - кривая.
// ---
std::string NURBSCurveSerializer::GetHeaderName()
{
  return Math::NurbsCurve().GetName();
}


//-----------------------------------------------------------------------------
//  Чтение nurbs - кривой из потока. Данные читаются в следующем порядке: количество опорных точек, опорные точки (Point),
// количество узлов, узлы, количество весов, веса, степень кривой, замкнутость.
// В случае невалидных данных возвращается пустой геометрический примитив.
// ---
std::unique_ptr<Math::Curve> NURBSCurveSerializer::Read( std::istream& theInput )
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
  std::vector<double> weights;
  numWeights = ReadSize_t( theInput );
  for ( size_t i = 0; i < numWeights; i++ )
  {
    weights.push_back( ReadDouble(theInput) );
  }

  // Чтение степени кривой.
  degree = ReadSize_t( theInput );
  // Чтение замкнутости кривой.
  isClosed =static_cast<bool>( ReadSize_t(theInput) );

  return std::make_unique<Math::NurbsCurve>( controlPoints, weights, isClosed, degree );

}


//-----------------------------------------------------------------------------
// Запись nurbs - кривой в поток. Данные записываются в следующем порядке: количество опорных точек, опорные точки (Point),
// количество узлов, узлы, количество весов, веса, степень кривой, замкнутость.
// В случае невалидных данных в файл ничего не записывается.
// ---
 void NURBSCurveSerializer::Write( std::ostream& theOutput, const Math::Curve& theCurve )
 {
  const Math::NurbsCurve& nurbsCurve =  static_cast<const Math::NurbsCurve&>( theCurve );
  const auto poles = static_cast<const Math::NurbsCurve&>( theCurve ).GetPoles();
  WriteSize_t( theOutput, poles.size() );
  for ( size_t i = 0; i < poles.size(); i++ )
  {
    WritePoint ( theOutput, poles[i] );
  }
  const auto weights = static_cast<const Math::NurbsCurve&>( theCurve ).GetWeights();

  WriteSize_t( theOutput, weights.size() );
  for ( size_t i = 0; i < weights.size(); i++ )
  {
    WriteDouble( theOutput, weights[i] );
  }
  WriteSize_t( theOutput,static_cast<const Math::NurbsCurve&> (theCurve).Degree() );
  WriteSize_t( theOutput,static_cast<const Math::NurbsCurve&> (theCurve).IsClosed() );

}
}
