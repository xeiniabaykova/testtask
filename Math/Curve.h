////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Базовый класс кривой.\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CURVE_H
#define CURVE_H
#include "GeometricPrimitive.h"
#include "CommonConstantsMath.h"
#include "Vector.h"
#include "Point.h"
#include "Range.h"
#include <vector>


namespace Math {
class GeomPolyline;
///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления кривой.
/**
  Абстрактный класс для кривой, представляющий возможность получить точку, первую и вторую производные
  границы парметра, венуть полилинию для гривой.
*/
///////////////////////////////////////////////////////////////////////////////
class Curve: public GeometricPrimitive {

public:
  Curve() = default;
  virtual ~Curve() = default;

private:
  Curve( const Curve &obj ) = delete;
  Curve& operator=( Curve &obj ) = delete;

public:
  virtual std::vector<Point> GetReferensedPoints () const = 0;               ///< Вернуть опорные точки, использованные для построения кривой.
  /// Вернуть точку на кривой по параметру t. Если параметр t выходит за допустимые границы параметра, то возвращается точка, соответствубщая ближайшей границе параметра.
  virtual Point              GetPoint            ( double t ) const = 0;
  virtual Range              GetRange            () const = 0;           ///< Вернуть допустимые границы параметра для кривой.
  virtual Vector             GetDerivative       ( double t ) const = 0; ///< Вернуть производную точки точки на кривой по параметру t.
  virtual Vector             Get2Derivative      ( double t ) const = 0; ///< вернуть вторую производную точки точки на кривой по параметру t.
  /// Возвращает полилинию для геометрического примитива с точностью accuracy. Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
  virtual void               GetAsPolyLine       ( GeomPolyline &polyLine, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;
  virtual bool               IsClosed            () const = 0;  /// Проверяется замкнутость геометрического примитива.
//protected:
   double                     FixParametr         ( double t ) const;        ///< Получить по параметру t, паремтр, врходящий в область определения для геометрического примитива.
private:
  /// Расчитывается параметрический шаг по кривой, позволяющий апроксимировать кривую с необходимой точностью.
  double CountingStep                   ( double tCurrent , double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;

};
}
#endif // C2CURVS_H
