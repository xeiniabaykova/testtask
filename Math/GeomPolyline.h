#ifndef GEOMPOLYLINE_H
#define GEOMPOLYLINE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Полилиния в двумерном пространстве.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "Curve.h"
#include "Point.h"
#include <vector>
#include "GeometricPrimitive.h"


namespace Math {
///////////////////////////////////////////////////////////////////////////////
/// Класс геометрического представления полилинии.
/**
  Представлет функции для хранения, масштабирования, поворота и сдвига геометрического объекта, проверки его на правильность.
*/
///////////////////////////////////////////////////////////////////////////////
class GeomPolyline: public Curve
{
private:
 std::vector<Point>  referencedPoints; ///< Опорные точки полилинии.
public:
 /**  \brief Конструктор полилинии по опорным точкам.
   \param[in] Points - опорные точки .\~
 */
 // ---
  GeomPolyline( const std::vector<Point>& thePoints );
  GeomPolyline();
  virtual ~GeomPolyline() = default;
  GeomPolyline( const GeomPolyline &obj ) = default;
  GeomPolyline& operator=( GeomPolyline &obj ) = default;
public:
  void                Init                ( const std::vector<Point>& theReferencedPoints ); ///< Заменить опорные точки в полилинии.
  virtual void        GetPoint             ( double t, Point& point ) const;                                ///< Вернуть точку по параметру t.
  virtual void        GetDerivative         ( double t, Vector& vector ) const;                ///< Вернуть производную линнии по параметру t.
  virtual void        Get2Derivative      ( double t, Vector& vector ) const;                                ///< Вернуть вторую производную на линии по параметру t.
  virtual bool        IsClosed            () const;                                          ///< Вернуть замкнутость полилинии.
  virtual Range       GetRange            () const;                                          ///< Вернуть парметризацию  параметризация от [0, 1].
  virtual void        Translate           ( double xShift, double yShift );                  ///< Сдвинуть по оси x на xShift, по оси y на yShift.
  virtual void        Rotate              ( double alpha );                                  ///< Повернуть полинию на угол alphaAng относительно начала координат.
  virtual void        Scale               ( double XScaling, double YScaling );              ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  /// Вернуть полилинию для полилинии (это и есть сама полилиния).
  virtual void        GetAsPolyLine       ( GeomPolyline &polyLine, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;
  double              DistanceToPoint     ( Point point ) const;                             ///< Вернуть расстояние от точки до полилинии.
  bool                IsValid             () const;                                          ///< Проверить корректность полилинии: нет совпадающих точек, количество точек не равно нулю.
  void                GetReferensedPoints ( std::vector<Point>& theReferensedPoints) const;     ///< Вернуть опорные точки, использованные для построения полилинии.
  virtual std::string GetName             () const;                                          ///< Вернуть имя, используемое при записи полилинии в файл.
  double              GetTFromPoint       ( const Point& point ) const;                      ///< Вернуть парметр t, соответсвующий точке на полилинии.
  /// Получить для заданной точности параметры использованные для построения полилинии.
  virtual void        GetReferensedParams( std::vector<double>& referensedParams, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;
  virtual CurveType   GetType             () const;
};
}
#endif // GEOMPOLYLINE_H
