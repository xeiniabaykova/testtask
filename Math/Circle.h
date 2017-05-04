////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  двумерная окружность\~

*/
////////////////////////////////////////////////////////////////////////////////

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
  /**  \brief \ru создать окружность по радиусу и центральной точке
    \param[in] center - центр окружности .\~
    \param[in] r - радиус окружности .\~
  */
  //---
  Circle ( Point center, double r);
  Circle() = delete;
  ~Circle() = default;

private:
  Circle( const Circle &obj ) = delete;
  Circle& operator=( Circle &obj ) = delete;

public:
  virtual Point GetPoint            ( double t ) const;                                             ///< вернуть точку на кривой по параметру t
  virtual Range GetRange            () const;                                                       ///< вернуть границы параметра для окружности : [0, 2*pi]
  virtual Point GetDerivativePoint  ( double t ) const;                                             ///< вернуть производную на окружности по параметру t
  virtual Point Get2DerivativePoint ( double t ) const;                                             ///< вернуть вторую производную на окружности по параметру t
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< вернуть полилинию для эллипса


};

#endif // CIRCLE_H
