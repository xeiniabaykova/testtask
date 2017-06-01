#ifndef VECTOR_H
#define VECTOR_H

#include "Math/GeometricPrimitive.h"
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru двумерный вектор \~

*/
////////////////////////////////////////////////////////////////////////////////

namespace Math {
class Vector: public GeometricPrimitive
{
private:
  double x; ///< координата x
  double y; ///< координата y

public:
  Vector() = default;
  Vector( double x, double y );
  Vector( const Vector& point );
  virtual ~Vector() = default;
public:
  virtual void Translate ( double xShift, double yShift );
  virtual void Rotate    ( double alpha );
  virtual void Scale ( double XScaling, double YScaling );
  double  GetX() const;
  double  GetY() const;
  Vector operator * ( double param  ) const;
  double operator * ( Vector param ) const;
  Vector operator - ( Vector point ) const;
  Vector operator + ( Vector point ) const;
  Vector operator = ( Vector point );
  bool IsValid() const;
};
}
#endif // VECTOR_H
