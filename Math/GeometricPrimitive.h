#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include <Math/Point.h>
#include <Math/Range.h>

//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для геометрического объекта \~
*/
// ---
class GeometricPrimitive { 
public:
  virtual Point GetPoint( double t ) const = 0;          ///< возвращает точку на кривой по параметру t
  virtual Range GetRange() const = 0;                    ///< возвращает допустимые границы параметра
  virtual Point GetDerivativePoint( double t ) const = 0; ///< возвращает производную точки точки на кривой по параметру t
  virtual ~GeometricPrimitive() = default;
};

#endif // GEOMETRICPRIMITIVE_H
