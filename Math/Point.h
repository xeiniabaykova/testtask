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
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс- заплатка для точки.
 * Используется для упрощения создания и отображения геометрических объектов  \~
**/
// ---
class Point: public GeometricPrimitive
{
private:
private:
	double x; ///< координата x
	double y; ///< координата y
  bool isValid;
public:
  //-------------------------------------------------------------------------------
  // \ru Создание точки по точке
  // ---
  Point():
    x(0),
    y(0){}
  Point ( double x, double y );
  Point(const Point& point);
  virtual void Translate ( double xShift, double yShift ); // сдвиг по осям x, y
  virtual void Rotate   ( double alpha ); // поворот на угол alpha
  virtual void Scale       ( double XScaling, double YScaling ); // матрица растяжения
  double  GetX() const; ///< получить координату x
  double  GetY() const; ///< получить координату y

  Point operator * ( double param ) const;
  Point operator - ( Point point ) const;
  Point operator + ( Point point ) const;
  Point operator = ( Point point );
  bool IsValid();
};


#endif // GEOMETRICPOINT_H
