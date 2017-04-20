#ifndef GEOMETRICPRIMITIVECREATOR_H
#define GEOMETRICPRIMITIVECREATOR_H
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
//
/// Абстрактный класс для отображения геометрического объекта
/**
*/
///////////////////////////////////////////////////////////////////////////////
///

class GeometricPrimitiveCreator {
public:
  virtual void Create( QChart * chart, const std::vector<QPoint>& points ) = 0;
};

#endif // GEOMETRICPRIMITIVECREATOR_H
