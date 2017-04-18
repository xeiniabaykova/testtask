#include "PointCreator.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtWidgets/QMainWindow>
#include <QMainWindow>


void PointCreator::CreatePoint( QChart * chart, QPointF Point )
{
    QScatterSeries *series0 = new QScatterSeries();
    series0->setName("scatter1");
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(15.0);


      series0->append(Point);
     series0->append(Point);
      chart->createDefaultAxes();
          chart->setDropShadowEnabled(false);
     //  series0->append(7, 4);
     //  series0->append(10, 5);
    chart->addSeries(series0);

}
