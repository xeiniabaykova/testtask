#ifndef CIRCLECREATOR_H
#define CIRCLECREATOR_H

#include "geometricprimitivecreator.h"
#include <functional>

class QPointF;
QT_CHARTS_USE_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
//  Класс отображения Эллипса на экране
///
/**
*/
///////////////////////////////////////////////////////////////////////////////
///


class EllipseCreator: public GeometricPrimitiveCreator {
public:
  virtual void Create( QChart * chart, const std::vector<QPoint>& points );
};

#endif // CIRCLECREATOR_H
