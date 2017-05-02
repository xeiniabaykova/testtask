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
private:
  GeometricPrimitive( const GeometricPrimitive &obj ) = delete;
  GeometricPrimitive& GeometricPrimitive::operator=( GeometricPrimitive &obj ) = delete;
public:
  GeometricPrimitive() = default;
  virtual Point GetPoint            ( double t ) const = 0;                                         ///< возвращает точку на кривой по параметру t
  virtual Range GetRange            () const = 0;                                                   ///< возвращает допустимые границы параметра
  virtual Point GetDerivativePoint  ( double t ) const = 0;                                         ///< возвращает производную точки точки на кривой по параметру t
  virtual Point Get2DerivativePoint ( double t ) const = 0;                                         ///< заплатка для общности: возвращает точку
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const = 0; ///< возвращает полилинию для геометрического примитива с точностью accuracy
  virtual ~GeometricPrimitive       () = default;
};

#endif // GEOMETRICPRIMITIVE_H
