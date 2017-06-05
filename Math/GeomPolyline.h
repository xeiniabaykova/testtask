////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Полилиния в двумерном пространстве.\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMPOLYLINE_H
#define GEOMPOLYLINE_H
#include "Curve.h"
#include "Point.h"
#include <vector>
#include "GeometricPrimitive.h"

///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления полилинии.
/**
  Представлет функции для хранения, масштабирования, поворота и сдвига геометрического объекта, проверки его на правильность.
*/
///////////////////////////////////////////////////////////////////////////////

namespace Math {
class GeomPolyline: public Curve
{
private:
 std::vector<Point> referencedPoints;
public:
 /**  \brief Конструктор полилинии по опорным точкам.
   \param[in] points - опорные точки .\~
 */
 //---
  GeomPolyline( const std::vector<Point>& thePoints );
  GeomPolyline();
  virtual ~GeomPolyline() = default;
private:
  GeomPolyline( const GeomPolyline &obj ) = delete;
  GeomPolyline& operator=( GeomPolyline &obj ) = delete;

public:
  void               Init                ( const std::vector<Point>& theReferencedPoints );
  virtual Point      GetPoint            ( double t ) const;                                             ///< Вернуть точку по параметру t.
  virtual Vector     GetDerivativePoint  ( double t ) const;                                             ///< Вернуть производную линнии по параметру t.
  virtual Vector     Get2DerivativePoint ( double t ) const;                                             ///< Вернуть вторую производную на линии по параметру t.
  virtual Range      GetRange            () const;                                                       ///< Вернуть парметризацию  параметризация от [0, 1].
  /// Вернуть полилинию для полилинии (это и есть сама полилиния).
  void               GetAsPolyLine(GeomPolyline &polyLine, double) const;
  double             DistanceToPoint     ( Point point ) const;                                           ///< Вернуть расстояние от точки до полилинии.
  virtual void       Translate           ( double xShift, double yShift );                                ///< Сдвинуть по оси x на xShift, по оси y на yShift.
  virtual void       Rotate              ( double alpha );                                                ///< Повернуть полинию на угол alphaAng относительно начала координат.
  virtual void       Scale               ( double XScaling, double YScaling );                            ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  bool               IsValid             () const;                                                        ///< Проверить корректность полилинии: нет совпадающих точек, количество точек не равно нулю.
  std::vector<Point> GetReferensedPoints () const;                                                        ///< Вернуть опорные точки, использованные для построения полилинии.
};
}
#endif // GEOMPOLYLINE_H
