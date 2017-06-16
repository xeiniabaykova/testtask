#ifndef GEOMETRICPOINT_H
#define GEOMETRICPOINT_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Точка в двумерном пространстве.\~

*/
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "GeometricPrimitive.h"


namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления точки.
/**
  Представлет функции для хранения и проведения опраций над точкой.
*/
///////////////////////////////////////////////////////////////////////////////
class Point: public GeometricPrimitive
{
private:
	double x; ///< координата x
	double y; ///< координата y
public:
  Point():
    x(0),
    y(0){}
  Point ( double x, double y );
  Point(const Point& point);
  Point(const Vector& vector);
  virtual void Translate   ( double xShift, double yShift );     ///< Сдвинуть по осям x, y.
  virtual void Rotate      ( double alpha );                     ///< Повернуть на угол alpha относительно начала координат.
  virtual void Scale       ( double XScaling, double YScaling ); ///< Маштабировать по оси х на xScaling, по оси y на yScaling.
  double       GetX        () const;                             ///< Получить координату x.
  double       GetY        () const;                             ///< Получить координату y.
  Point        operator *  ( double param ) const;               ///< Умножить точку на скаляр.
  Point        operator /  ( double param ) const;               ///< Разделить точку на скаляр.
  Vector       operator -  ( Point point ) const;                ///< Получить разность точек ( по определению, это вектор).
  Point        operator +  ( Point point ) const;                ///< Сложить две точки.
  Point        operator += ( Point point );                      ///< Присвоить к текущей точке другую точку.
  Point        operator +  ( Vector vector ) const;              ///< Сложить ветор и точку. По определению, это точка.
  Point        operator =  ( Point point );                      ///< Прировнять одну точку к другой точке.
  bool         IsValid     () const;                             ///< Проверить точки точки на правильность.
  std::string  GetName     () const;                             ///< Вернуть имя, используемое при записи точки в файл.
};

 double Distance ( Point point1, Point point2 ); ///< Найти расстояние между точками.
 bool   IsEqual   ( Point point1, Point point2 ); ///< Проверить точки на эквивалентность.
}
#endif // GEOMETRICPOINT_H
