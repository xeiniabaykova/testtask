#include "Polyline.h"
#include "Math/GeometricPoint.h"
#include "Math/Line.h"
#include <cmath>

//-----------------------------------------------------------------------------
/**
  \ru На вход отдается геометрический примитив и точность создания полилинии
*/
//-----------------------------------------------------------------------------
Polyline::Polyline( const GeometricPrimitive* figure, double precision ):
  figure( figure ),
  precision( precision ),
  displayedPoints(0)
{
}


//-----------------------------------------------------------------------------
/**
  \ru Расчитываются точки для отображения: Если интервала нулевой, считаем, что это одна
  точка, возращаем ее.
  Если интервал ненулевой, то обходи его с приращением, которое определяем на каждом шаге
*/
//-----------------------------------------------------------------------------
std::vector<Point> Polyline::GetPoints()
{
  // если не точка и не прямая, то применяем общий алгоритм
  double t = figure->GetRange().GetStart();
  displayedPoints.push_back( figure->GetPoint(t) );
  while ( t <= figure->GetRange().GetEnd() )
  {
    t += CountingStep( t );
    Point point ( figure->GetPoint(t) );
    displayedPoints.push_back( point );
  }
   return displayedPoints;
}

//-----------------------------------------------------------------------------
/**
  \ru приращение параметра t
*/
//-----------------------------------------------------------------------------
double Polyline::CountingStep( double tCurrent )
{
  precision = 0.01;
  Point firstDerivative = figure->GetDerivativePoint( tCurrent );
  Point secondDerivative = figure->Get2DerivativePoint( tCurrent );
  double vectorMult = firstDerivative.GetX() * secondDerivative.GetY() - firstDerivative.GetY() * secondDerivative.GetX();
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double normFirstDerivative = sqrt( firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY() );
  double multiplicationFirstDerivative = firstDerivative.GetX() * firstDerivative.GetX() +  firstDerivative.GetY() * firstDerivative.GetY();
  double deltaT = 2 * sqrt ( precision * (2 * normFirstDerivative / normVectorMult - precision / multiplicationFirstDerivative) );
  return deltaT;
}


