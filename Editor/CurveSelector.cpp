#include "CurveSelector.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <cmath>
#include <QtWidgets/QMessageBox>

namespace {

double Dot( Point point1, Point point2 )
{
  return point1.GetX() * point2.GetX() + point1.GetY() * point2.GetY();
}

double Distance( Point point1, Point point2 )
{
  return sqrt ( (point1.GetX() - point2.GetX()) * (point1.GetX() - point2.GetX()) +
         (point1.GetY() - point2.GetY()) * (point1.GetY() - point2.GetY()) );
}

//-----------------------------------------------------------------------------
/**
  \ru расстояние от точки до прямой.
  \ru QPoint first, QPoint second - начальные и конечные точки прямой
  \ru point - точка, до которой находится расстояние
*/
//-----------------------------------------------------------------------------
 double Distance( Point first, Point second, Point point )
 {
       Point v = second - first;
       Point w = point - first;

       double c1 = Dot( w, v );
       if ( c1 <= 0 )
           return Distance( point, first );

       double c2 = Dot(v,v);
       if ( c2 <= c1 )
           return Distance( point, second );

       double b = c1 / c2;
       Point Pb = first + v * b;
       return Distance( point, Pb );
 }
}

CurveSelector::CurveSelector( const std::vector<std::vector<Point>>& points ):
  points( points ) {}


//-----------------------------------------------------------------------------
/**
  \ru обходим все полилинии в chart
  \ru до каждого отрезка полилинии находим расстояние. Если оно меньше допустимого отклонения, то возвращаем порядковый номер series в chart
  \ru point - точка, до которой находится расстояние
  \ru если ни одна series не подошла, возвращаем -1
*/
//-----------------------------------------------------------------------------
int CurveSelector::GetCurve( Point point )
{


  for ( int i = 0; i < points.size(); i++ )
  {
    for ( int j = 1; j < points[i].size(); j++ )
    {
      if (Distance( points[i][j - 1], points[i][j], point)  < 0.1)
       return i;
    }
  }
  return -1;
}
