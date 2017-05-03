////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  двумерный эллипс\~

*/
////////////////////////////////////////////////////////////////////////////////

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
  Point center;  ///< центр
  double r1;     ///< первый радиус
  double r2;     ///< второй радиус
public:
  /**  \brief \ru создать эллипс по двум радиусам и центральной точке
    \param[in] center - центр эллипса .\~
    \param[in] r1 - главный радиус .\~
    \param[in] r2 - побочный радиус .\~
  */
  //---
  Ellipse ( Point center, double r1, double r2 );
  Ellipse () = delete;
  ~Ellipse() = default;

private:
  Ellipse( const Ellipse &obj ) = delete;
  Ellipse& Ellipse::operator=( Ellipse &obj ) = delete;
public:
  virtual Point GetPoint            ( double t                                             ) const; ///< вернуть точку на кривой по параметру t
  virtual Range GetRange            (                                                      ) const; ///< вернуть границы параметра для эллипса : [0, 2*pi]
  virtual Point GetDerivativePoint  ( double t                                             ) const; ///< вернуть производную на эллипсе по параметру t
  virtual Point Get2DerivativePoint ( double t                                             ) const; ///< вернуть вторую производную на эллипсе по параметру t
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< вернуть полилинию для эллипса

};

#endif // ELLIPSE_H
