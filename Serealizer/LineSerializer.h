#ifndef LINESERIALIZER_H
#define LINESERIALIZER_H

#include <Math/Line.h>
#include "CurveSerializer.h"


class LineSerializer :public CurveSerializer {
public:
  LineSerializer() = default;
  virtual std::unique_ptr<Math::GeometricPrimitive> Read          ( std::istream& theInput );
  virtual void                                Write         ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve);
  virtual std::string                         GetHeaderName ();
  virtual ~LineSerializer() = default;
};

#endif // LINESERIALIZER_H
