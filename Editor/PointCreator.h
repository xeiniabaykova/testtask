#ifndef POINTCREATOR_H
#define POINTCREATOR_H

#include "GeometricPrimitiveCreator.h"
#include <QPointF>

QT_CHARTS_USE_NAMESPACE


//------------------------------------------------------------------------------
/** \brief \ru Класс отображения точки. \~
*/
// ---
class PointCreator: public GeometricPrimitiveCreator {
public:
  virtual void Create( QChart * chart, const std::vector<QPoint>& points );
};

#endif // POINTCREATOR_H
