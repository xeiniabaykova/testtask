#include "PrintFigure.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QMessageBox>


//-----------------------------------------------------------------------------
/**
  \ru сохранение QChart
*/
//-----------------------------------------------------------------------------
PrintFigure::PrintFigure( QChart * chart ):
  chart( chart ) {
  axisX = new QValueAxis;
  axisX->setRange( 0, 10 );
  chart->addAxis(axisX, Qt::AlignBottom);

  axisY = new QValueAxis;
  axisY->setRange( 0, 10 );
  chart->addAxis(axisY, Qt::AlignLeft);
  chart->legend()->setVisible(false);
  QLineSeries *series = new QLineSeries;
  *series<<QPointF( 0, 0 )<<QPointF( 10, 10);
  series->setColor(QColor(255,255,255) );
  chart->addSeries( series );
  chart->axisX()->setVisible( false );
  chart->axisY()->setVisible( false );
}


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
    *currentseries <<QPointF(points[i].GetX(), points[i].GetY());

  for ( int i = 0; i < refPoints.size(); i++ )
    *seriesRef << QPointF( refPoints[i].GetX(), refPoints[i].GetY());

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );

  currentseries->attachAxis( axisX );
  currentseries->attachAxis( axisY );
  seriesRef->attachAxis( axisX );
  seriesRef->attachAxis( axisY );
}
