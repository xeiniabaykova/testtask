#include "DisplayedFigure.h"
#include "Math/GeometricPoint.h"
#include "Math/Line.h"
#include <cmath>

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
  if ( dynamic_cast<GeometricPoint*>(figure.get()) != nullptr ) { // если точка, то добавляем одну точку в получаемую полилинию
    Point point ( figure->GetPoint(figure->GetRange().GetEnd()) );
    displayedPoints.push_back( point );
    return displayedPoints;
  } else if ( dynamic_cast<Line*>(figure.get()) ) {
    Point pointBegin( figure->GetPoint(figure->GetRange().GetStart()) ); // если линия, то только две точки в полилинию: начало и конец
    Point pointEnd( figure->GetPoint(figure->GetRange().GetEnd()) );
    displayedPoints.push_back( pointBegin );
    displayedPoints.push_back( pointEnd );
    return displayedPoints;
  } else { // если не точка и не прямая, то применяем общий алгоритм
    double t = figure->GetRange().GetStart();
    while ( t < figure->GetRange().GetEnd() ){
      Point point ( figure->GetPoint(t) );
      displayedPoints.push_back( point );
      t += CountingStep( t );
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


//-----------------------------------------------------------------------------
/**
  \ru приращение параметра t
*/
//-----------------------------------------------------------------------------
double DisplayedFigure::CountingStep( double tCurrent )
{
  precision = 0.01;
  Point firstDerivative = figure->GetDerivativePoint(tCurrent);
  Point secondDerivative = figure->Get2DerivativePoint(tCurrent);
  double vectorMult = firstDerivative.GetX() * secondDerivative.GetY() - firstDerivative.GetY() * secondDerivative.GetX();
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double normFirstDerivative = sqrt( firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY() );
  double multiplicationFirstDerivative = firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY();
  double deltaT = 2 * sqrt ( precision * (2 * normFirstDerivative / normVectorMult - precision / multiplicationFirstDerivative) );
  return deltaT;
}


