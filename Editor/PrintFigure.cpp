#include "PrintFigure.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QMessageBox>
#include <Math/GeometricPrimitive.h>


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
  series->setColor( QColor(255,255,255) );
  chart->addSeries( series );
  seriesReferenced = new QScatterSeries();
  chart->addSeries( seriesReferenced );
  seriesReferenced->attachAxis( axisX );
  seriesReferenced->attachAxis( axisY );
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
void PrintFigure::AddFigure( std::shared_ptr<DisplayedCurve> curve, QColor color ) 
{

  QLineSeries * currentseries = new QLineSeries();
  currentseries->setColor( color );

  QScatterSeries *seriesRef = new QScatterSeries();
  seriesRef->setColor( color );
  std::vector<Point> polyPoints;
  curve->primitive->GetAsPolyLine( polyPoints, 0.01 );

  for ( int i = 0; i < polyPoints.size(); i++ )
    *currentseries <<QPointF( polyPoints[i].GetX(), polyPoints[i].GetY() );

  for ( int i = 0; i < curve->referencedPoints.size(); i++ )
    *seriesRef << QPointF( curve->referencedPoints[i].GetX(), curve->referencedPoints[i].GetY());

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );
  //seriesReferenced->clear();
  // seriesReferenced = new QScatterSeries();

  currentseries->attachAxis( axisX );
  currentseries->attachAxis( axisY );
  seriesRef->attachAxis( axisX );
  seriesRef->attachAxis( axisY );
}


void PrintFigure::AddReferencedPoint( Point point, QColor color )
{
  QScatterSeries *seriesRef = new QScatterSeries();
  seriesRef->setColor( color );
  *seriesRef << QPointF( point.GetX(), point.GetY() );
  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( seriesRef );
  seriesRef->attachAxis( axisX );
  seriesRef->attachAxis( axisY );
}
