#ifndef LINE_H
#define LINE_H
#include <Math/Point.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class Line: public GeometricPrimitive
{
private:
  Point startPoint; ///< начальная точка
  Point direction;  ///< направление
public:
  //-------------------------------------------------------------------------------
  // \ru создание линии по двум точкам: началу и концу
  // ---
  Line( Point startPoint, Point endPoint );
  virtual Point GetPoint( double t ) const;            ///< возвращает точку по параметру t
  virtual Point GetDerivativePoint( double t ) const;  ///< возвращает производную линнии по параметру t
  virtual Point Get2DerivativePoint( double t ) const; ///< возвращает вторую производную на линии по параметру t
  virtual Range GetRange() const;                      ///< возвращает парметризацию  параметризация от [0, 1]
};

#endif // LINE_H
