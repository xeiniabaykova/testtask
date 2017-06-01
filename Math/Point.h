////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  точка в двумерном пространстве (возможно, больше не нужно)\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPOINT_H
#define GEOMETRICPOINT_H
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс- заплатка для точки.
 * Используется для упрощения создания и отображения геометрических объектов  \~
**/
// ---
namespace Math {
class Point: public GeometricPrimitive
{
private:
private:
	double x; ///< координата x
	double y; ///< координата y
public:
  //-------------------------------------------------------------------------------
  //  Создание точки по точке
  // ---
  Point():
    x(0),
    y(0){}
  Point ( double x, double y );
  Point(const Point& point);
  Point(const Vector& vector);
  virtual void Translate ( double xShift, double yShift ); // сдвиг по осям x, y
  virtual void Rotate    ( double alpha ); // поворот на угол alpha
  virtual void Scale     ( double XScaling, double YScaling ); // матрица растяжения
  double       GetX      () const; ///< получить координату x
  double       GetY      () const; ///< получить координату y

  Point operator * ( double param ) const;
  Vector operator - ( Point point ) const;
  Point operator + ( Point point ) const;
  Point operator + ( Vector vector ) const;
  Point operator = ( Point point );
  double Distance( Point point1, Point point2 ) const;
  bool IsValid() const;
};

 double Distance( Point point1, Point point2 );
 bool IsEqual   ( Point point1, Point point2 );
}
#endif // GEOMETRICPOINT_H
