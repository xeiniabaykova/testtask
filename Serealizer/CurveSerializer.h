#ifndef CURVESERIALIZER_H
#define CURVESERIALIZER_H
#pragma once
#include "Math/GeometricPrimitive.h"
#include <istream>
#include <memory>

class CurveSerializer {
public:
  virtual std::unique_ptr<GeometricPrimitive> Read (std::istream& theInput) = 0;
  virtual void Write (std::ostream& theOutput, const GeometricPrimitive& theCurve) = 0;
  virtual ~CurveSerializer() = default;

  Point ReadPoint   ( std::istream& input ) const;
  void WritePoint   ( std::ostream& output, const Point& point ) const;
  double ReadDouble ( std::istream& input ) const;
  double ReadInt    ( std::istream& input ) const;
  void WriteInt     ( std::ostream& output, int value ) const;
  void WriteDouble  ( std::ostream& otutput, double value ) const;
};

#endif // CURVESERIALIZER_H
