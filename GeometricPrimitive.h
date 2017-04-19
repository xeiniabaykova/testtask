#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include <QPoint>

class GeometricPrimitive {
    virtual QPoint GetPoint( double t ) = 0;
    virtual QPoint GetRange() = 0;
};

#endif // GEOMETRICPRIMITIVE_H
