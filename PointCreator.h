#ifndef POINTCREATOR_H
#define POINTCREATOR_H

#include "GeometricPrimitiveCreator.h"
#include <QPointF>
QT_CHARTS_USE_NAMESPACE
class PointCreator: public GeometricPrimitiveCreator {
public:
    void CreatePoint( QChart * chart, QPointF Point);
};

#endif // POINTCREATOR_H
