#include "LineCreator.h"
#include "Math/Line.h"
#include <QtCharts/QLineSeries>


//------------------------------------------------------------------------------
// \ru отображение линии на экране по двум точкам, создание геометрического примитива линия
// ---
void LineCreator::Create(QChart * chart, const std::vector<QPoint>& points )
{
  QLineSeries *series0 = new QLineSeries();
  Line currentLine( points );
  *series0 << chart->mapToValue( points[0] );
  *series0 << chart->mapToValue( points[1] );

  chart->addSeries( series0 );
  chart->createDefaultAxes();
  chart->setDropShadowEnabled( false );

}
