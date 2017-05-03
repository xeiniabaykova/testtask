#include "DisplayedCurve.h"



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

std::vector<Point>& DisplayedCurve::GetReferensedPoints()
{
 return referencedPoints;
}

std::vector<Point>& DisplayedCurve::GetPolyline()
{
  return polyline;
}

std::shared_ptr<GeometricPrimitive>& DisplayedCurve::GetPrimitive()
{
 return primitive;
}

bool DisplayedCurve::GetSelectionStatus()
{
  return selected;
}

void DisplayedCurve::ModifySelectionStatus(Point cursor)
{
  presision=0.1;
 if ( DistanceToPoint(cursor) < presision )
   selected = !selected;
 if (selected)
   SetColor( selectedColor );
 else
   SetColor( currentColor );
}

double DisplayedCurve::DistanceToPoint( Point point )
{
  double maxDistance = std::numeric_limits<double>::max();
  for ( int i = 1; i< polyline.size(); i++ )
  {
    if ( Distance(polyline[i-1], polyline[i], point) < maxDistance  )
      maxDistance = Distance( polyline[i-1], polyline[i], point );
  }
  return maxDistance;
}

void DisplayedCurve::SetColor( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
 // currentColor = color;
}

void DisplayedCurve::SetSeries( QLineSeries *  current, QScatterSeries *ref )
{
  currentseries = current;
  seriesRef = ref;
}

 DisplayedCurve::~DisplayedCurve()
{
  chart->removeSeries( currentseries );
  chart->removeSeries( seriesRef);
}
