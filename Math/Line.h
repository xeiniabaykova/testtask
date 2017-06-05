////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Отрезок в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef LINE_H
#define LINE_H
#include "Curve.h"
#include "Point.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"


///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления отрезка.
/**
  Представлет функции для хранения и проведения опраций над отрезком.
*/
///////////////////////////////////////////////////////////////////////////////
namespace Math {
class Line: public Curve
{
private:
  Point startPoint; ///< Начальная точка отрезка.
  Point endPoint;  ///< Конечная точка отрезка.
public:
   Line() = delete;
   /**  \brief Создать отрезок по вектору из точек.
     \param[in] points - набор точек .\~
      Если количество точек в массиве меньше двух, то отрезок не создается.
      Если количество точек больше двух, то отрезок создается по первым двум точкам, где
      первая точка - точка начала отрезка, вторая, точка конца отрезка.
      Если точки совпадают между собой. то отрезок не создается.
   */
   //---
  Line( const std::vector<Point>& points );
  /**  \brief создать отрезок по начальной и конечной точке отрезка
    \param[in] startPoint - начальная точка .\~
    \param[in] endPoint - конечная точка .\~
     Если точки совпадают между собой. то отрезок не создается.
  */
  //---
  Line( Point startPoint, Point endPoint );
  virtual ~Line() = default;

public:
  virtual Point              GetPoint            ( double t ) const;                                             ///< Вернуть точку по параметру t.
  virtual Vector             GetDerivativePoint  ( double t ) const;                                             ///< Вернуть производную линнии по параметру t.
  virtual Vector             Get2DerivativePoint ( double t ) const;                                             ///< Вернуть вторую производную на линии по параметру t.
  virtual Range              GetRange            () const;                                                       ///< Вернуть парметризацию  параметризация от [0, 1].
  virtual void               GetAsPolyLine       ( GeomPolyline &polyLine, double ) const;                       ///< Вернуть полилинию для линии (это две точки - начало и конец).
  virtual void               Translate           ( double xShift, double yShift );                               ///< Сдвинуть по осям x, y.
  virtual void               Rotate              ( double alpha );                                               ///< Повернуть на угол alpha относительно начала координат.
  virtual void               Scale               ( double xScaling, double yScaling );                           ///< Маштабировать по оси х на xScaling, по оси y на yScaling.
  std::string                GetName             () const;                                                       ///< Вернуть имя, используемое при записи отрезка в файл.
  bool                       IsValid             () const;                                                       ///< Проверка на корректность отрезка (начальная и конечная точка не совпадают).
  virtual std::vector<Point> GetReferensedPoints () const;                                                       ///< Вернуть опорные точки, использованные для построения отрезка.

private:
  Line( const Line &obj ) = delete;
  Line& operator=( Line &obj ) = delete;

};
}
#endif // LINE_H
