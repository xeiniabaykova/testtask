////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Двумерный вектор.\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H
#define VECTOR_H

#include "GeometricPrimitive.h"
namespace Math {
////////////////////////////////////////////////////////////////////////////////
/**

   Класс, представляющий собой двумерный вектор. \~

*/
////////////////////////////////////////////////////////////////////////////////
class Vector: public GeometricPrimitive
{
private:
  double x; ///< Координата x.
  double y; ///< Координата y.

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
  double       VectorMult ( Vector vector ) const;               ///< Получить векторное произведение векторов по определению.
  Vector       operator * ( double param ) const;                ///< Домножить вектор на скаляр.
  double       operator * ( Vector param ) const;                ///< Получить скалярное произведение векторов по определению.
  Vector       operator - ( Vector vector ) const;               ///< Получить разность векторов.
  Vector       operator + ( Vector vector ) const;               ///< Получить сумму векторов.
  Vector       operator = ( Vector vector );                     ///< Присвоить один вектор другому.
  bool         IsValid    () const;                              ///< Проверить вектор на правильность.
  double       Lenght     () const;                              ///< Получить длину вектора по определению.   
  bool         IsCollinear( Vector vector) const;                ///< Проверить вектора на параллельность.
  std::string  GetName    () const;                              ///< Вернуть имя, используемое при записи точки в файл.
};
}
#endif // VECTOR_H
