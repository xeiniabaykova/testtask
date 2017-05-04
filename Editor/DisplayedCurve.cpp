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


//-----------------------------------------------------------------------------
/**
  \ru вернуть опорные точки кривой
*/
//-----------------------------------------------------------------------------
std::vector<Point>& DisplayedCurve::GetReferensedPoints()
{
 return referencedPoints;
}


//-----------------------------------------------------------------------------
/**
  \ru вернуть отображаемую полилинию
*/
//-----------------------------------------------------------------------------
const GeomPolyline& DisplayedCurve::GetPolyline() const
{
  return polyline;
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
void DisplayedCurve::ModifySelectionStatus( Point cursor )
{
 if ( DistanceToPoint(cursor) < precision )
    selected = !selected;
 if (selected)
   SetColor( selectedColor );
 else
   SetColor( currentColor );
}


//-----------------------------------------------------------------------------
/**
  \ru измерить расстояние от текущей точки до полилинии
*/
//-----------------------------------------------------------------------------
double DisplayedCurve::DistanceToPoint( Point point )
{
  double minDistance = std::numeric_limits<double>::max();
  std::vector<Point> polylinePoints;
  polyline.GetAsPolyLine( polylinePoints, precision );

  for ( int i = 1; i< polylinePoints.size(); i++ )
  {
    if ( Distance(polylinePoints[i-1], polylinePoints[i], point) < minDistance  )
      minDistance = Distance( polylinePoints[i-1], polylinePoints[i], point );
  }
  return minDistance;
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
  currentseries = current;
  seriesRef = ref;
}


//-----------------------------------------------------------------------------
/**
  \ru убрать series из графика
*/
//-----------------------------------------------------------------------------
 DisplayedCurve::~DisplayedCurve()
{
  chart->removeSeries( currentseries );
  chart->removeSeries( seriesRef);
  delete currentseries;
  delete seriesRef;
}
