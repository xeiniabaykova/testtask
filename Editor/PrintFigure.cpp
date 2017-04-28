#include "PrintFigure.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMessageBox>


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
void PrintFigure::AddFigure( const std::vector<Point>& points, const std::vector<Point>& refPoints, QColor color ) {

  QLineSeries * currentseries = new QLineSeries();
  currentseries->setColor( color );

  QScatterSeries *seriesRef = new QScatterSeries();
  seriesRef->setColor( color );

  for ( int i = 0; i < points.size(); i++ )
    *currentseries <<QPointF(points[i].GetX(), points[i].GetY())  ;

  for ( int i = 0; i < refPoints.size(); i++ )
    *seriesRef << QPointF( refPoints[i].GetX(), refPoints[i].GetY())  ;
  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );

   chart->addSeries( currentseries );
   chart->addSeries( seriesRef );
chart->createDefaultAxes();
   chart->axisX()->setMin(0);
   chart->axisX()->setMax(10);

   chart->axisY()->setMin(0);
   chart->axisY()->setMax(10);


    chart->axisX()->setVisible(false);
    chart->axisY()->setVisible(false);
}
