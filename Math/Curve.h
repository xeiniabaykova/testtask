#ifndef CURVE_H
#define CURVE_H
#include "GeometricPrimitive.h"
#include "CommonConstantsMath.h"
#include "Vector.h"


#include "Point.h"
#include "Range.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления эллипса.
/**
  Абстрактный класс для кривой.
*/
///////////////////////////////////////////////////////////////////////////////
///
///
namespace Math {
class Curve: public GeometricPrimitive {
public:
  Curve() = default;
  virtual ~Curve() = default;
private:
  Curve( const Curve &obj ) = delete;
  Curve& operator=( Curve &obj ) = delete;
public:
  std::vector<Point> GetReferensedPoints() const;
  void SetReferensedPoints           ( const std::vector<Point>& points );
  virtual Point   GetPoint            ( double t ) const = 0;                                             ///< вернуть точку на кривой по параметру t
  virtual Range   GetRange            () const = 0;                                                       ///< вернуть допустимые границы параметра
  virtual Vector  GetDerivativePoint  ( double t ) const = 0;                                             ///< вернуть производную точки точки на кривой по параметру t
  virtual Vector  Get2DerivativePoint ( double t ) const = 0;                                             ///< вернуть вторую производную точки точки на кривой по параметру t
  virtual void    GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy = CommonConstantsMath::PRECISION_POLYLINE) const; ///< возвращает полилинию для геометрического примитива с точностью accuracy
  double CountingStep                ( double tCurrent , double accuracy = CommonConstantsMath::PRECISION_POLYLINE) const;

};
}
#endif // C2CURVS_H
