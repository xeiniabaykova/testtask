#ifndef CURVESELECTOR_H
#define CURVESELECTOR_H

#include <Math/Point.h>
#include <vector>
#include <QtCharts/QChartView>
QT_CHARTS_USE_NAMESPACE

//------------------------------------------------------------------------------
/** \brief \ru Класс для создания селектирования кривых, отображенных на экране. \~
*/
// ---
class CurveSelector
{
private:
  const std::vector<std::vector<Point>>& points;
public:
  CurveSelector( const std::vector<std::vector<Point>>& points );
  int GetCurve( Point point ); ///< отдает номер series в chart, у которой расстояние меньше допустимого
};

#endif // CURVESELECTOR_H
