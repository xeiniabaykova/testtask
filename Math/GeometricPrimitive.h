////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  базовый класс геометрического примитива в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include <Math/Point.h>
#include <Math/Range.h>
#include <vector>

//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для геометрического объекта \~
*/
// ---
class GeometricPrimitive { 
public:
  GeometricPrimitive() = default;
  virtual ~GeometricPrimitive  () = default;
private:
  GeometricPrimitive( const GeometricPrimitive &obj ) = delete;
  GeometricPrimitive& operator=( GeometricPrimitive &obj ) = delete;
public:
  virtual Point GetPoint            ( double t ) const = 0;                                             ///< вернуть точку на кривой по параметру t
  virtual Range GetRange            () const = 0;                                                       ///< вернуть допустимые границы параметра
  virtual Point GetDerivativePoint  ( double t ) const = 0;                                             ///< вернуть производную точки точки на кривой по параметру t
  virtual Point Get2DerivativePoint ( double t ) const = 0;                                             ///< вернуть вторую производную точки точки на кривой по параметру t
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const = 0; ///< возвращает полилинию для геометрического примитива с точностью accuracy
};

#endif // GEOMETRICPRIMITIVE_H
