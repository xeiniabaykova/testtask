#ifndef C2CURVE_H
#define C2CURVE_H
#include "GeometricPrimitive.h"

#include <Math/Point.h>
#include <Math/Range.h>
#include <vector>

//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для дважды дифференцируемой кривой  \~
*/
// ---
class C2Curve: public GeometricPrimitive {
  std::vector<Point> referencedPoints;
public:
  C2Curve() = default;
  virtual ~C2Curve() = default;
private:
  C2Curve( const C2Curve &obj ) = delete;
  C2Curve& operator=( C2Curve &obj ) = delete;
public:
  std::vector<Point> GetReferensedPoints() const;
  void SetReferensedPoints           ( const std::vector<Point>& points );
  virtual Point  GetPoint            ( double t ) const = 0;                                             ///< вернуть точку на кривой по параметру t
  virtual Range  GetRange            () const = 0;                                                       ///< вернуть допустимые границы параметра
  virtual Point  GetDerivativePoint  ( double t ) const = 0;                                             ///< вернуть производную точки точки на кривой по параметру t
  virtual Point  Get2DerivativePoint ( double t ) const = 0;                                             ///< вернуть вторую производную точки точки на кривой по параметру t
  virtual void   GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const = 0; ///< возвращает полилинию для геометрического примитива с точностью accuracy
  virtual double DistanceToPoint     ( Point point) const = 0;
  double DistancePointToCurve        ( Point point, const std::vector<Point>& polylinePoints ) const;  ///< возвращает расстояние от точки до полилилинии геометрического примитива
};

#endif // C2CURVS_H
