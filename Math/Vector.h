////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Двумерный вектор.\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H
#define VECTOR_H

#include "GeometricPrimitive.h"
////////////////////////////////////////////////////////////////////////////////
/**
  
   Класс, представляющий собой двумерный вектор. \~

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
  virtual void Translate  ( double xShift, double yShift );      ///< Сдвинуть по осям x, y на xShift, yShift .
  virtual void Rotate     ( double alpha );                      ///< Повернуть на угол alpha относительно начала координат.
  virtual void Scale      ( double XScaling, double YScaling );  ///< Маштабировать по оси х на xScaling, по оси y на yScaling.
  double       GetX       () const;                              ///< Получить координату x.
  double       GetY       () const;                              ///< Получить координату y.
  Vector       operator * ( double param ) const;                ///< Домножить вектор на скаляр.
  double       operator * ( Vector param ) const;                ///< Получить скалярное произведение векторов по определению.
  Vector       operator - ( Vector vector ) const;               ///< Получить разность векторов.
  Vector       operator + ( Vector vector ) const;               ///< Получить сумму векторов.
  Vector       operator = ( Vector vector );                      ///< Присвоить один вектор другому.
  bool         IsValid    () const;                              ///< Проверить вектор на правильность.
};
}
#endif // VECTOR_H
