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
  int numNodes;
  int numWeights;
  int numPoles;
  int degree;
  int isClosed;
  numPoles = ReadInt( theInput );
// Чтение опорных точек.
  std::vector<Math::Point> controlPoints; 
  for (size_t i = 0; i < numPoles; i++)
  {
    controlPoints.push_back ( ReadPoint(theInput) );
  }

// Чтение узлов.
  std::vector<double> nodes;
  numNodes = ReadInt( theInput );
  for ( size_t i = 0; i < numNodes; i++ )
  {
    nodes.push_back ( ReadDouble(theInput) );
  }

  std::vector<double> weights;
  numWeights = ReadInt( theInput );
  for ( size_t i = 0; i < numWeights; i++ )
  {
    weights.push_back( ReadDouble(theInput) );
  }

// Чтение степени кривой.
  degree = ReadInt( theInput );
  // Чтение замкнутости кривой.
  isClosed = ReadInt( theInput );

  return std::make_unique<Math::NurbsCurve>( controlPoints, weights, nodes, isClosed, degree );

}


//-----------------------------------------------------------------------------
// Запись nurbs - кривой в поток. Данные записываются в следующем порядке: количество опорных точек, опорные точки (Point),
// количество узлов, узлы, количество весов, веса, степень кривой, замкнутость.
// В случае невалидных данных в файл ничего не записывается.
// ---
 void NURBSCurveSerializer::Write( std::ostream& theOutput, const Math::Curve& theCurve )
 {
  auto poles = static_cast<const Math::NurbsCurve&>( theCurve ).GetPoles();
  WriteInt( theOutput, static_cast<int>(poles.size()) );
  for ( size_t i = 0; i < poles.size(); i++ )
  {
    WritePoint ( theOutput, poles[i] );
  }
  auto nodes = static_cast<const Math::NurbsCurve&>( theCurve ).GetNodes();
  WriteInt( theOutput, static_cast<int>(nodes.size()) );
  for ( size_t i = 0; i < nodes.size(); i++ )
  {
    WriteDouble( theOutput, nodes[i] );
  }
  auto weights = static_cast<const Math::NurbsCurve&>( theCurve ).GetWeights();

  WriteInt( theOutput, static_cast<int>(weights.size()) );
  for (size_t i = 0; i < weights.size(); i++)
  {
    WriteDouble( theOutput, weights[i] );
  }

  WriteInt( theOutput,static_cast<const Math::NurbsCurve&> (theCurve).Degree() );
  WriteInt( theOutput,static_cast<const Math::NurbsCurve&> (theCurve).IsClosed() );
}
}
