#include "PrintFigure.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>


//-----------------------------------------------------------------------------
/**
  \ru сохранение QChart
*/
//-----------------------------------------------------------------------------
PrintFigure::PrintFigure( QChart * chart ):
  chart( chart ) {}


//-----------------------------------------------------------------------------
/**
  \ru добавление новой кривой:
  если это точка, то рисуем QScatterSeries,
  если это набор точек, то отображаем как QLineSeries (точки, соединенные линиями)
*/
//-----------------------------------------------------------------------------
void PrintFigure::AddFigure( const std::vector<Point>& points ) {

  if ( points.size() == 1 ) {
    QScatterSeries *series0 = new QScatterSeries();
    series0->setColor( QColor( 51, 0, 51 ) );
    series0->setPointLabelsVisible(false);
    series0->setMarkerShape( QScatterSeries::MarkerShapeCircle );
    series0->setMarkerSize( 15.0 );
    *series0 << chart->mapToValue( QPoint(points[0].GetX(), points[0].GetY()) ) ;
    chart->addSeries( series0 );
    chart->createDefaultAxes();
  } else {
     QLineSeries * currentseries = new QLineSeries();
      currentseries->setColor( QColor( 51, 0, 51 ) );
    for ( int i = 0; i < points.size(); i++ )
        *currentseries << chart->mapToValue( QPoint(points[i].GetX(), points[i].GetY()) ) ;
    chart->addSeries( currentseries );
    chart->createDefaultAxes();
    chart->setDropShadowEnabled( false );
  }

  chart->axisX()->setVisible(false);
  chart->axisY()->setVisible(false);
}
