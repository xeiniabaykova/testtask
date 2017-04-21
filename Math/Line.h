#ifndef LINE_H
#define LINE_H
#include <functional>
#include <QPoint>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class Line: public GeometricPrimitive
{
private:
  QPointF startPoint;
  QPointF direction;
public:
  Line( const std::vector<QPoint>& points );
  QPointF GetPoint( double t );
  QPointF GetRange();
};

#endif // LINE_H
