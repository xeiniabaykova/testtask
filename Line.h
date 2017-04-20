#ifndef LINE_H
#define LINE_H
#include <functional>
#include <QPoint>
#include "GeometricPrimitive.h"

class Line: public GeometricPrimitive
{
public:
  Line();
  QPoint GetPoint( double t );
  QPoint GetRange();
};

#endif // LINE_H
