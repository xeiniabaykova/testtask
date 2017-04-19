#ifndef POINT_H
#define POINT_H
#include <functional>
#include <QPoint>
#include "GeometricPrimitive.h"

class Point:  public GeometricPrimitive
{
public:
    Point( QPoint point );
    QPoint GetPoint( double t );
    QPoint GetRange();
};

#endif // POINT_H
