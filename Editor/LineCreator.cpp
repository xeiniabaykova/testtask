#include "LineCreator.h"
#include <QtCharts/QLineSeries>


void LineCreator::Create(QChart * chart, const std::vector<QPoint>& points )
{
  QLineSeries *series0 = new QLineSeries();
  series0->append( points[0] );
  series0->append( points[1] );
  chart->addSeries( series0 );
  chart->createDefaultAxes();
  chart->setDropShadowEnabled( false );

}
