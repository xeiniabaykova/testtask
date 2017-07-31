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
 std::vector<Point>  referencedPoints;
 std::vector<double> referemcedParams;

public:
 /**  \brief Конструктор полилинии по опорным точкам.
   \param[in] Points - опорные точки .\~
 */
 // ---
  GeomPolyline( const std::vector<Point>& thePoints );
  GeomPolyline();
  virtual ~GeomPolyline() = default;

private:
  GeomPolyline( const GeomPolyline &obj ) = delete;
  GeomPolyline& operator=( GeomPolyline &obj ) = delete;

public:
  void                Init                ( const std::vector<Point>& theReferencedPoints, const std::vector<double>& refParam = std::vector<double>() ); ///< Заменить опорные точки в полилинии.
  virtual Point       GetPoint            ( double t ) const;                                ///< Вернуть точку по параметру t.
  virtual Vector      GetDerivative       ( double t ) const;                                ///< Вернуть производную линнии по параметру t.
  virtual Vector      Get2Derivative      ( double t ) const;                                ///< Вернуть вторую производную на линии по параметру t.
  virtual bool        IsClosed            () const;                                          ///< Вернуть замкнутость полилинии.
  virtual Range       GetRange            () const;                                          ///< Вернуть парметризацию  параметризация от [0, 1].
  virtual void        Translate           ( double xShift, double yShift );                  ///< Сдвинуть по оси x на xShift, по оси y на yShift.
  virtual void        Rotate              ( double alpha );                                  ///< Повернуть полинию на угол alphaAng относительно начала координат.
  virtual void        Scale               ( double XScaling, double YScaling );              ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  /// Вернуть полилинию для полилинии (это и есть сама полилиния).
  virtual void        GetAsPolyLine       ( GeomPolyline &polyLine, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;
  double              DistanceToPoint     ( Point point ) const;                             ///< Вернуть расстояние от точки до полилинии.
  bool                IsValid             () const;                                          ///< Проверить корректность полилинии: нет совпадающих точек, количество точек не равно нулю.
  std::vector<double> GetReferensedParams() const;                                           ///< Получить значения параметра исходной кривой по которому строилась полилилния.
  std::vector<Point>  GetReferensedPoints () const;                                          ///< Вернуть опорные точки, использованные для построения полилинии.
  virtual std::string GetName             () const;                                          ///< Вернуть имя, используемое при записи полилинии в файл.
  virtual CurveType   GetType             () const;
};
}
#endif // GEOMPOLYLINE_H
