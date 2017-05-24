////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  двумерный эллипс\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "C2Curve.h"
#include "Point.h"
#include "Range.h"
#include <vector>
#include "GeometricPrimitive.h"
#include "Vector.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления эллипса \~
*/
// ---
class Ellipse: public C2Curve
{
private:
  Point center;  ///< центр
  double r1;     ///< первый радиус
  double r2;     ///< второй радиус
  double alpha;  ///< угол между главной осью и осью х
public:
  /**  \brief \ru создать эллипс по двум радиусам и центральной точке
    \param[in] center - центр эллипса .\~
    \param[in] r1 - главный радиус .\~
    \param[in] r2 - побочный радиус .\~
  */
  //---
  Ellipse ( const std::vector<Point>& points );
  Ellipse ( Point center, double r1, double r2, double alpha );
  Ellipse () = default;
  virtual ~Ellipse() = default;

private:
  Ellipse( const Ellipse &obj ) = delete;
  Ellipse& operator=( Ellipse &obj ) = delete;
public:
  virtual Point  GetPoint            ( double t ) const;                                               ///< вернуть точку на кривой по параметру t
  virtual Range  GetRange            () const;                                                         ///< вернуть границы параметра для эллипса : [0, 2*pi]
  virtual Vector  GetDerivativePoint  ( double t ) const;                                               ///< вернуть производную на эллипсе по параметру t
  virtual Vector  Get2DerivativePoint ( double t ) const;                                               ///< вернуть вторую производную на эллипсе по параметру t
  virtual double DistanceToPoint     ( Point point ) const;                                            ///< возвращает расстояние от точки до полилинии эллипса
  virtual void   Translate         ( double xShift, double yShift );
  virtual void   Rotate            ( double alpha );
  virtual void   Scale             ( double XScaling, double YScaling );

  bool   IsValid() const;
  double GetMajorRadius() const { return r1; }
  double GetMinorRadius() const { return r2; }
  double GetAlpha() const { return alpha; }
  Point  GetCenter() const { return center; }
  std::string GetName() const;
};

#endif // ELLIPSE_H
