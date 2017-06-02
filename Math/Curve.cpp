#include "Curve.h"
#include <cmath>
#include <limits>
namespace Math {
//-----------------------------------------------------------------------------
/**
  Расчитывается параметрический шаг по кривой, позволяющий апроксимировать кривую с необходимой точностью.
*/
//---
double Curve::CountingStep( double tCurrent, double accuracy) const
{
  Vector firstDerivative = GetDerivativePoint( tCurrent );
  Vector secondDerivative = Get2DerivativePoint( tCurrent );
  double vectorMult = firstDerivative * secondDerivative;
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double multiplicationFirstDerivative = firstDerivative * firstDerivative;
  double normFirstDerivative = sqrt( multiplicationFirstDerivative );
  return 2 * sqrt (accuracy * (2 * normFirstDerivative / normVectorMult - accuracy / multiplicationFirstDerivative) );
}


//-----------------------------------------------------------------------------
/**
  Возвращает полилинию для геометрического примитива с точностью accuracy. Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
*/
//---
void Curve::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  polyLinePoints.clear();
  double t = GetRange().GetStart();
  polyLinePoints.push_back( GetPoint(t) );
  while ( t < GetRange().GetEnd() )
  {
    t += CountingStep( t, accuracy);
    Point point ( GetPoint(t) );
    polyLinePoints.push_back( point );
  }
}
}
