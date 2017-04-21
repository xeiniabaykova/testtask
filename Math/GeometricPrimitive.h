#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include <QPoint>


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для геометрического объекта \~
*/
// ---
class GeometricPrimitive {
  virtual QPointF GetPoint( double t ) = 0;
  virtual QPointF GetRange() = 0;
};

#endif // GEOMETRICPRIMITIVE_H
