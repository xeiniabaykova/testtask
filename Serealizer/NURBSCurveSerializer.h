#ifndef NURBSCURVESERIALIZER_H
#define NURBSCURVESERIALIZER_H
#include <Math/NurbsCurve.h>
#include "CurveSerializer.h"

class NURBSCurveSerializer :public CurveSerializer {
public:
  NURBSCurveSerializer() = default;
  virtual std::unique_ptr<Math::GeometricPrimitive> Read          ( std::istream& theInput );
  virtual void                                Write         ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve );
  virtual std::string                         GetHeaderName ();

  virtual ~NURBSCurveSerializer() = default;
};
#endif
