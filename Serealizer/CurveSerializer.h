#ifndef CURVESERIALIZER_H
#define CURVESERIALIZER_H
#include "Math/GeometricPrimitive.h"#include "Math/Point.h"
#include <istream>
#include <memory>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

class CurveSerializer {
public:
  virtual std::unique_ptr<GeometricPrimitive> Read (std::istream& theInput) = 0;
  virtual void Write (std::ostream& theOutput, const GeometricPrimitive& theCurve) = 0;
  virtual ~CurveSerializer() = default;

  Point ReadPoint   ( std::istream& input ) const;
  void WritePoint   ( std::ostream& output, const Point& point ) const;
  double ReadDouble ( std::istream& input ) const;
  int ReadInt    ( std::istream& input ) const;
  void WriteInt     ( std::ostream& output, int value ) const;
  void WriteDouble  ( std::ostream& otutput, double value ) const;
};

#endif // CURVESERIALIZER_H
