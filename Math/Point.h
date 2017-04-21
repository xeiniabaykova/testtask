#ifndef POINT_H
#define POINT_H
#include <functional>
#include <QPoint>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления точки \~
*/
// ---
class Point:  public GeometricPrimitive {
private:
  QPointF point;
public:
  Point  ( QPointF point );
  QPointF GetPoint( double t );
  QPointF GetRange();
};

#endif // POINT_H
