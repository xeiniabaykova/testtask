#include "DisplayedFigure.h"


//-----------------------------------------------------------------------------
/**
  \ru На вход отдается геометрический примитив
*/
//-----------------------------------------------------------------------------
DisplayedFigure::DisplayedFigure( std::shared_ptr<GeometricPrimitive> figure ):
  figure( figure ),
  displayedPoints(0) {}


//-----------------------------------------------------------------------------
/**
  \ru Расчитываются точки для отображения: Если интервала нулевой, считаем, что это одна
  точка, возращаем ее.
  Если интервал ненулевой, то обходи его с постоянным шагов с количеством разбиений равным 1000
*/
//-----------------------------------------------------------------------------
std::vector<Point> DisplayedFigure::GetPoints()
{
  int numPoints = 1000;
  double stepSize = ( figure->GetRange().GetEnd() - figure->GetRange().GetStart() ) / numPoints ;

  if ( stepSize == 0 ) { // если точка
     Point point ( figure->GetPoint(figure->GetRange().GetEnd()) );
     displayedPoints.push_back( point );
     return displayedPoints;
  } else {
    for ( int i = 0; i < numPoints; i++ )
    {
      Point currentPoint( figure->GetPoint(figure->GetRange().GetStart() + i * stepSize) );
      displayedPoints.push_back( currentPoint );
    }
    return displayedPoints;
   }
}


//-----------------------------------------------------------------------------
/**
  \ru возвращаем геометрический примитив
*/
//-----------------------------------------------------------------------------
std::shared_ptr<GeometricPrimitive> DisplayedFigure::GetFigure()
{
  return figure;
}


