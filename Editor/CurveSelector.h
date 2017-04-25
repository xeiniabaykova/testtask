#ifndef CURVESELECTOR_H
#define CURVESELECTOR_H

#include <Math/Point.h>
#include <QtCharts/QChartView>
QT_CHARTS_USE_NAMESPACE

//------------------------------------------------------------------------------
/** \brief \ru Класс для создания селектирования кривых, отображенных на экране. \~
*/
// ---
class CurveSelector
{
private:
  QChart *chart;
public:
  CurveSelector( QChart *chart );
  int GetCurve( Point point ); ///< отдает номер series в chart, у которой расстояние меньше допустимого
};

#endif // CURVESELECTOR_H
