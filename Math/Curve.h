#ifndef CURVE_H
#define CURVE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Базовый класс кривой.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "GeometricPrimitive.h"
#include "CommonConstantsMath.h"
#include "Vector.h"
#include "Point.h"
#include "Range.h"
#include <vector>


namespace Math {
class GeomPolyline;
class Line;
class Ellipse;
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
  enum CurveType {
    LineType,
    PolylineType,
    CircleType,
    EllipseType,
    NurbsType
  };

  virtual std::vector<Point> GetReferensedPoints () const = 0; ///< Вернуть опорные точки, использованные для построения кривой.
  /// Вернуть точку на кривой по параметру t. Если параметр t выходит за допустимые границы параметра, то возвращается точка, соответствубщая ближайшей границе параметра.
  virtual Point              GetPoint            ( double t ) const = 0;
  virtual Range              GetRange            () const = 0;           ///< Вернуть допустимые границы параметра для кривой.
  virtual Vector             GetDerivative       ( double t ) const = 0; ///< Вернуть производную точки точки на кривой по параметру t.
  virtual Vector             Get2Derivative      ( double t ) const = 0; ///< Вернуть вторую производную точки точки на кривой по параметру t.
  /// Вернуть полилинию для геометрического примитива с точностью accuracy. Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
  virtual void               GetAsPolyLine       ( GeomPolyline &polyLine, std::vector<double>& refParam = std::vector<double>(), double accuracy = CommonConstantsMath::PRECISION_POLYLINE) const;
  virtual bool               IsClosed            () const = 0;  /// Проверить замкнутость геометрического примитива.
  virtual CurveType          GetType             () const = 0;  /// Получить тип кривой.

protected:
   void                      FixParameter        ( double& t ) const;        ///< Получить по параметру t, паремтр, врходящий в область определения для геометрического примитива.
private:
  /// Расчитать параметрический шаг по кривой, позволяющий апроксимировать кривую с необходимой точностью.
  double CountingStep                            ( double tCurrent , double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;

};
}
#endif // C2CURVS_H
