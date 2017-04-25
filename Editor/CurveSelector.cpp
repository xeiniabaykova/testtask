#include "CurveSelector.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <cmath>

namespace {

//-----------------------------------------------------------------------------
/**
  \ru расстояние от точки до прямой.
  \ru QPoint first, QPoint secound - начальные и конечные точки прямой
  \ru point - точка, до которой находится расстояние
*/
//-----------------------------------------------------------------------------
 double Distance( QPointF first, QPointF secound, Point point )
 {
   double a = first.y() - secound.y();
   double b = secound.x() - first.x();
   double c = first.x() * secound.y() - secound.x() * first.y();
   double dist = ( a * point.GetX() + b * point.GetY() + c ) / sqrt ( a*a + b*b );
   return sqrt( dist * dist );
 }
}

CurveSelector::CurveSelector( QChart *chart ):
  chart( chart ) {}


//-----------------------------------------------------------------------------
/**
  \ru обходим все полилинии в chart
  \ru до каждого отрезка полилинии находим расстояние. Если оно меньше допустимого отклонения, то возвращаем порядковый номер series в chart
  \ru point - точка, до которой находится расстояние
  \ru если ни одна series не подошла, возвращаем -1
*/
//-----------------------------------------------------------------------------
int CurveSelector::GetCurve (Point point)
{

  for ( int i = 0; i < chart->series().size(); i++ ) {
   QXYSeries * currentSeries = static_cast<QXYSeries *> ( chart->series()[i] );
  for ( int j = 1; j < currentSeries->points().size(); j++)
    if ( Distance(currentSeries->points()[j - 1], currentSeries->points()[j], point) < 0.1 ) {
        return i;
    }
  }
 return -1;
}
