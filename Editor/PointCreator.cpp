#include "PointCreator.h"
#include "Math/Point.h"
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>


//-----------------------------------------------------------------------------
/**
  \ru отображение точки на экране
*/
//-----------------------------------------------------------------------------
void PointCreator::Create( QChart * chart, const std::vector<QPoint>& points )
{
  QScatterSeries *series0 = new QScatterSeries();
  Point currentPoint( points[0] );

  series0->setName( "scatter1" );
  series0->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  series0->setMarkerSize( 15.0 );
  *series0 << chart->mapToValue( points[0] ) ;

  chart->addSeries( series0 );
  chart->createDefaultAxes();

  chart->setDropShadowEnabled( false );
}
