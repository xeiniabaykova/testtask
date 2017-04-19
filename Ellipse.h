#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <QPoint.h>
#include <vector>
#include "GeometricPrimitive.h"

class Ellipse: public GeometricPrimitive
{
private:
    QPoint center;
    double r1;
    double r2;
public:
    Ellipse( const std::vector<QPoint>& points );
    QPoint GetPoint( double t );
    QPoint GetRange();
};

#endif // ELLIPSE_H
