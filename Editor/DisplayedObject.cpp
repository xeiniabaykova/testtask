#include "DisplayedObject.h"

namespace {

double Dot( Point point1, Point point2 )
{
  return point1.GetX() * point2.GetX() + point1.GetY() * point2.GetY();
}

//-----------------------------------------------------------------------------
/**
  \ru находим квадрат растояния между двумя точками
*/
//-----------------------------------------------------------------------------
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

   double c2 = Dot( v, v );
   if ( c2 <= c1 )
       return Distance( point, second );

   double b = c1 / c2;
   Point Pb = first + v * b;
   return Distance( point, Pb );
}
}


//-----------------------------------------------------------------------------
/**
  \ru вернуть текущее состояние селекции
*/
//-----------------------------------------------------------------------------
bool DisplayedCurve::GetSelectionStatus()
{
  return selected;
}


//-----------------------------------------------------------------------------
/**
  \ru изменить состояние селекции в зависимости от полученной точки
*/
//-----------------------------------------------------------------------------
void DisplayedCurve::ModifySelectionStatus( Point cursor, double precision, QColor selectedColor )
{
 if ( curve->DistancePointToCurve( cursor ) < precision )
    selected = !selected;
 if (selected)
   SetColor( selectedColor );
 else
   SetColor( currentColor );
}


//-----------------------------------------------------------------------------
/**
  \ru установить новый цвет для неселектированной кривой
*/
//-----------------------------------------------------------------------------
void DisplayedCurve::SetColor( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
}

void DisplayedCurve::SetColorUnselectedCurve( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
  currentColor = color;
}

//-----------------------------------------------------------------------------
/**
  \ru добавить series к кривой
*/
//-----------------------------------------------------------------------------
void DisplayedCurve::SetSeries( QLineSeries *  current, QScatterSeries *ref )
{
  currentseries = std::make_shared<QLineSeries>( current);
  seriesRef = std::make_shared<QScatterSeries>(ref);
}


//-----------------------------------------------------------------------------
/**
  \ru убрать series из графика
*/
//-----------------------------------------------------------------------------
 DisplayedCurve::~DisplayedCurve()
{
  currentseries->chart()->removeSeries( currentseries.get() );
  currentseries->chart()->removeSeries( seriesRef.get() );
}

void DisplayedCurve::addCurveToChart( QChart * chart, double accuracy )
{
  QLineSeries * currentseries = new QLineSeries();
  currentseries->setColor( currentColor );

  QScatterSeries *seriesRef = new QScatterSeries();
  seriesRef->setColor( currentColor );

  std::vector<Point> polyPoints;
  curve->GetAsPolyLine( polyPoints,accuracy );
  for ( int i = 0; i < polyPoints.size(); i++ )
  *currentseries <<QPointF( polyPoints[i].GetX(), polyPoints[i].GetY() );

  for ( int i = 0; i < curve->GetReferensedPoints().size(); i++ )
      *seriesRef << QPointF( curve->GetReferensedPoints()[i].GetX(), curve->GetReferensedPoints()[i].GetY() );
  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );


 // currentseries->attachAxis( axisX );
//  currentseries->attachAxis( axisY );
 // seriesRef->attachAxis( axisX );
//  seriesRef->attachAxis( axisY );

}
