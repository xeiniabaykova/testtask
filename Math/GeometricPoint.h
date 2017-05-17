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
class GeometricPoint: public GeometricPrimitive
{
private:
  Point point; ///< точка
  bool isValid;
public:
  //-------------------------------------------------------------------------------
  // \ru Создание точки по точке
  // ---
  GeometricPoint ( Point point );
  virtual void Translation ( double xShift, double yShift ); // сдвиг по осям x, y
  virtual void Rotation    ( double alpha ); // поворот на угол alpha
  virtual void Dilatation  ( double XScaling, double YScaling ); // матрица растяжения
  bool IsValid();
};


#endif // GEOMETRICPOINT_H
