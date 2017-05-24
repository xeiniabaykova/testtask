#include "NURBSCurveSerializer.hxx"

std::string NURBSCurveSerializer::GetHeaderName()
{
  return NurbsCurve().GetName();
}

  std::unique_ptr<GeometricPrimitive> NURBSCurveSerializer::Read( std::istream& theInput )
  {

  int  numNodes;
  int numWeights;
  int numPoles;
  int degree;
  int isClosed;
  numPoles = ReadInt( theInput );
  std::vector<Point> controlPoints;
  for (size_t i = 0; i < numPoles; i++)
  {
    controlPoints.push_back ( ReadPoint(theInput) );
  }


  std::vector<double> nodes;
  numNodes = ReadInt( theInput );
  for ( size_t i = 0; i < numNodes; i++ )
  {
    nodes.push_back ( ReadDouble(theInput) );
  }

  std::vector<double> weights;
  numWeights = ReadInt( theInput );
  for ( size_t i = 0; i < numNodes; i++ )
  {
    weights.push_back( ReadDouble(theInput) );
  }

  degree = ReadInt( theInput );
  isClosed = ReadInt( theInput );
  return std::make_unique<NurbsCurve>( controlPoints, weights, nodes, isClosed, degree );

}

 void NURBSCurveSerializer::Write ( std::ostream& theOutput, const GeometricPrimitive& theCurve )
 {
  auto poles = static_cast<const NurbsCurve&>(theCurve).GetPoles();
  WriteInt(theOutput, (int)poles.size());
  for ( size_t i = 0; i < poles.size(); i++ )
  {
    WritePoint ( theOutput, poles[i] );
  }
  auto nodes = static_cast<const NurbsCurve&>( theCurve ).GetNodes();
  WriteInt( theOutput, (int)nodes.size() );
  for ( size_t i = 0; i < nodes.size(); i++ )
  {
    WriteDouble( theOutput, nodes[i] );
  }
  auto weights = static_cast<const NurbsCurve&>( theCurve ).GetWeights();

  WriteInt( theOutput, (int)weights.size() );
  for (size_t i = 0; i < weights.size(); i++)
  {
    WriteDouble( theOutput, weights[i] );
  }

  WriteInt( theOutput,static_cast<const NurbsCurve&> (theCurve).Degree() );
  WriteInt( theOutput,static_cast<const NurbsCurve&> (theCurve).IsClosed() );
}