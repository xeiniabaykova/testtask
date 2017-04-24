#ifndef GEOMETRICPOINT_H
#define GEOMETRICPOINT_H
#include <vector>
#include "Point.h"
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс- заплатка для точки.
 * Используется для упрощения создания и отображения геометрических объектов  \~
**/
// ---
class GeometricPoint: public GeometricPrimitive
{
private:
  Point point; ///< точка
public:
  //-------------------------------------------------------------------------------
  // \ru Создание точки по точке
  // ---
  GeometricPoint( Point point );
  virtual Point GetPoint( double t ) const;            ///< заплатка для общности: возвращает точку
  virtual Point GetDerivativePoint( double t ) const;  ///< заплатка для общности: возвращает точку
  virtual Point Get2DerivativePoint( double t ) const; ///< заплатка для общности: возвращает точку
  virtual Range GetRange() const;                      ///< заплатка для общности: возвращает нулевую парамтризацию
};


#endif // GEOMETRICPOINT_H
