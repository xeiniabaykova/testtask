#include "EllipseCreator.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include "Math/Ellipse.h"
#include <QtWidgets/QMessageBox>


//------------------------------------------------------------------------------
// \ru Создает геометрический примитив эллипс и отрисоывает его на экране
// ---
void EllipseCreator::Create( QChart * chart, const std::vector<QPoint>& points )
{
  QLineSeries * ellipseseries = new QLineSeries();
  int colSteps = 10;
  Ellipse current( points );
  double steprange = abs( current.GetRange().y() - current.GetRange().x() ) / colSteps;


  for ( int i = 0; i <= colSteps; i++ )
  {
    *ellipseseries <<  chart->mapToValue( current.GetPoint(i * steprange) ) ;
  }

  chart->addSeries( ellipseseries );
  chart->createDefaultAxes();
  chart->setDropShadowEnabled( false );
}
