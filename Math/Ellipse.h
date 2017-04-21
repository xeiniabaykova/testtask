#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <QPoint>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления эллипса \~
*/
// ---
class Ellipse: public GeometricPrimitive
{
private:
  QPoint center;
  double r1;
  double r2;
public:
  Ellipse ( const std::vector<QPoint>& points );
  QPointF  GetPoint( double t );
  QPointF  GetRange();
};

#endif // ELLIPSE_H
