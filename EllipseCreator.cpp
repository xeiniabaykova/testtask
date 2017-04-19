#include "EllipseCreator.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include "Ellipse.h"

void EllipseCreator::Create( QChart * chart, const std::vector<QPoint>& points )
{
    QSplineSeries * ellipseseries = new QSplineSeries;
    int colSteps = 100;
    Ellipse current( points );
    double steprange = abs( current.GetRange().y() - current.GetRange().x() ) / colSteps;

    for ( int i = 0; i < colSteps; i++ )
    {
       ellipseseries->append( current.GetPoint( i * steprange ) );
    }

    chart->addSeries( ellipseseries );
    chart->createDefaultAxes();
    chart->setDropShadowEnabled( false );
}
