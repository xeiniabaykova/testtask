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

  virtual void     GetReferensedPoints  ( std::vector<Point>& referensedPoints ) const = 0; ///< Вернуть опорные точки, использованные для построения кривой.
  /// Вернуть точку на кривой по параметру t. Если параметр t выходит за допустимые границы параметра, то возвращается точка, соответствубщая ближайшей границе параметра.
  virtual void    GetPoint            ( double t, Point& point) const = 0;
  virtual Range   GetRange            () const = 0;           ///< Вернуть допустимые границы параметра для кривой.
  virtual void    GetDerivative        ( double t, Vector& vector ) const = 0; ///< Вернуть производную точки точки на кривой по параметру t.
  virtual void    Get2Derivative      ( double t, Vector& vector ) const = 0; ///< Вернуть вторую производную точки точки на кривой по параметру t.
  /// Вернуть полилинию для геометрического примитива с точностью accuracy. Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
  virtual void      GetAsPolyLine       ( GeomPolyline &polyLine, double accuracy = CommonConstantsMath::PRECISION_POLYLINE) const;
  virtual bool      IsClosed            () const = 0;  /// Проверить замкнутость геометрического примитива.
  virtual CurveType GetType             () const = 0;  /// Получить тип кривой.
  /// Получить для заданной точности параметры использованные для построения полилинии.
  virtual void     GetReferensedParams ( std::vector<double>& referensedParams, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;

protected:
   void            FixParameter        ( double& t ) const;        ///< Получить по параметру t, паремтр, врходящий в область определения для геометрического примитива.
private:
  /// Расчитать параметрический шаг по кривой, позволяющий апроксимировать кривую с необходимой точностью.
  double CountingStep                  ( double tCurrent , double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;

};
}
#endif // C2CURVS_H
