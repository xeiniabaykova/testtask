#ifndef CIRCLE_H
#define CIRCLE_H
#include <Math/Point.h>
#include <Math/Range.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления окружности \~
*/
// ---
class Circle: public GeometricPrimitive
{
private:
  Point center; ///< центр
  double r;     ///< радиус
public:
  //-------------------------------------------------------------------------------
  // \ru создание окружности по точке в центре и радиусу
  // ---
  Circle ( Point center, double r);
  virtual Point GetPoint            ( double t ) const;                                             ///< возвращает точку на кривой по параметру t
  virtual Range GetRange            () const;                                                       ///< возвращает границы параметра для окружности : [0, 2*pi]
  virtual Point GetDerivativePoint  ( double t ) const;                                             ///< возвращает производную на окружности по параметру t
  virtual Point Get2DerivativePoint ( double t ) const;                                             ///< возвращает вторую производную на окружности по параметру t
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< возвращает полилинию для эллипса
};

#endif // CIRCLE_H
