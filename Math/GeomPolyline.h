////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  Полилиния в двумерном пространстве.\~

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
 /**  \brief \ru Конструктор полилинии по опорным точкам.
   \param[in] points - опорные точки .\~
 */
 //---
  GeomPolyline( const std::vector<Point>& points );
  GeomPolyline() = delete;
  virtual ~GeomPolyline() = default;
private:
  GeomPolyline( const GeomPolyline &obj ) = delete;
  GeomPolyline& operator=( GeomPolyline &obj ) = delete;

public:
  virtual Point  GetPoint            ( double t ) const;                                             ///< Вернуть точку по параметру t.
  virtual Vector GetDerivativePoint  ( double t ) const;                                             ///< Вернуть производную линнии по параметру t.
  virtual Vector Get2DerivativePoint ( double t ) const;                                             ///< Вернуть вторую производную на линии по параметру t.
  virtual Range  GetRange            () const;                                                       ///< Вернуть парметризацию  параметризация от [0, 1].
  virtual void   GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< Вернуть полилинию для полилинии (это и есть сама полилиния).
  virtual double DistanceToPoint     ( Point point ) const;                                           ///< Вернуть расстояние от точки до полилинии.
  virtual void   Translate           ( double xShift, double yShift );                                ///< Сдвиг по оси x на xShift, по оси y на yShift.
  virtual void   Rotate              ( double alpha );                                                ///< Повернуть эллипс на угол alphaAng относительно начала координат.
  virtual void   Scale               ( double XScaling, double YScaling );                            ///< Масштабировать на xScaling по оси x, на yScaling по оси у
  bool           IsValid             () const;
};
}
#endif // GEOMPOLYLINE_H
