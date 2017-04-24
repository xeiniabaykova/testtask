#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <Math/Point.h>
#include <Math/Range.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления эллипса \~
*/
// ---
class Ellipse: public GeometricPrimitive
{
private:
  Point center; ///< центр
  double r1;     ///< первый радиус
  double r2;     ///< второй радиус
public:
  //-------------------------------------------------------------------------------
  // \ru создание эллипса по точке в центре и двум радиусам
  // ---
  Ellipse ( Point center, double r1, double r2 );
  virtual Point   GetPoint( double t ) const;                ///< возвращает точку на кривой по параметру t
  virtual Range   GetRange() const;                          ///< возвращает границы параметра для эллипса : [0, 2*pi]
};

#endif // ELLIPSE_H
