#ifndef ELLIPSECURVESERIALIZER_H
#define ELLIPSECURVESERIALIZER_H
#include <Ellipse.h>
#include "CurveSerializer.h"
#include <ostream>

class EllipseCurveSerializer :public CurveSerializer {
	
public:
  EllipseCurveSerializer() = default;
  virtual std::unique_ptr<GeometricPrimitive>Read( std::istream& theInput );
  virtual void Write( std::ostream& theOutput, const GeometricPrimitive& theCurve );
	virtual std::string GetHeaderName();
  virtual ~EllipseCurveSerializer() = default;
};

#endif
