#ifndef LINE_H
#define LINE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Отрезок в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "Curve.h"
#include "Point.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"


namespace Math {

///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления отрезка.
/**
  Представлет функции для хранения и проведения опраций над отрезком.
*/
///////////////////////////////////////////////////////////////////////////////
class Line: public Curve
{
private:
  Point startPoint; ///< Начальная точка отрезка.
  Point endPoint;  ///< Конечная точка отрезка.

public:
   Line( const Line &obj );
   Line() = default;
   /**  \brief Создать отрезок по вектору из точек.
     \param[in] points - набор точек .\~
      Если количество точек в массиве меньше двух, то отрезок не создается.
      Если количество точек больше двух, то отрезок создается по первым двум точкам, где
      первая точка - точка начала отрезка, вторая, точка конца отрезка.
      Если точки совпадают между собой. то отрезок не создается.
   */
   //---
  Line( const std::vector<Point>& points );
  /**  \brief Cоздать отрезок по начальной и конечной точке отрезка
    \param[in] startPoint - начальная точка .\~
    \param[in] endPoint - конечная точка .\~
     Если точки совпадают между собой. то отрезок не создается.
  */
  //---
  Line( Point startPoint, Point endPoint );
  virtual ~Line() = default;

public:
  virtual void              GetPoint             ( double t, Point& point ) const;         ///< Вернуть точку по параметру t.
  virtual void              GetDerivative        ( double t, Vector& vector ) const;                       ///< Вернуть производную линнии по параметру t.
  virtual Vector             Get2Derivative      ( double t ) const;                       ///< Вернуть вторую производную на линии по параметру t.
  virtual Range              GetRange            () const;                                 ///< Вернуть парметризацию  параметризация от [0, 1].
  virtual void               GetAsPolyLine       ( GeomPolyline &polyLine, double ) const; ///< Вернуть полилинию для линии (это две точки - начало и конец).
  virtual void               Translate           ( double xShift, double yShift );         ///< Сдвинуть по осям x, y.
  virtual void               Rotate              ( double alpha );                         ///< Повернуть на угол alpha относительно начала координат.
  virtual void               Scale               ( double xScaling, double yScaling );     ///< Маштабировать по оси х на xScaling, по оси y на yScaling.
  virtual bool               IsClosed            () const;                                 ///< Вернуть замкнутость отрезка. Отрезок всегда не замкунут.
  std::string                GetName             () const;                                 ///< Вернуть имя, используемое при записи отрезка в файл.
  bool                       IsValid             () const;                                 ///< Проверить корректность отрезка (начальная и конечная точка не совпадают).
  virtual void               GetReferensedPoints ( std::vector<Point>& referensedPoints) const; ///< Вернуть опорные точки, использованные для построения отрезка.
  Point                      GetStartPoint       () const;                                 ///< Вернуть начальную точку отрезка.
  Point                      GetEndPoint         () const;                                 ///< Вернуть конечную точку отрезка.
  double                     DistanceToPoint     ( Point point ) const;                    ///< Вернуть расстояние от точки до прямой.
  virtual CurveType          GetType             () const;                                 ///< Вернуть тип кривой. 
  bool                       operator==          ( const Line& obj) const;                 ///< Сравнить отрезки на эквивалентность.
  Line&                      operator=           ( const Line &obj);                       ///< Присвоить один отрезок другому отрезку.
  double                     GetTFromPoint       ( const Point& point ) const;             ///< Получить параметр t по точке, принаджлежащей прямой.
  bool                       IsPointInLine       ( const Point& point ) const;             ///< Проверить, принадлежит ли точка отрезку.
  /// Получить для заданной точности параметры использованные для построения полилинии.
  virtual void               GetReferensedParams( std::vector<double>& referensedParams, double accuracy = CommonConstantsMath::PRECISION_POLYLINE ) const;
};
double Distance( const Line& line1, const Line& line2, Point& closestPoint );                ///< Вернуть расстояние между отрезками.
}
#endif // LINE_H
