#include "Curve.h"
#include <cmath>
#include <limits>
#include "GeomPolyline.h"
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
  double vectorMult = firstDerivative.GetX() * secondDerivative.GetY() - firstDerivative.GetY() * secondDerivative.GetX();
  double normVectorMult = sqrt( vectorMult * vectorMult );
  double multiplicationFirstDerivative = firstDerivative * firstDerivative;
  double normFirstDerivative = sqrt( multiplicationFirstDerivative );
  return 2 * sqrt ( accuracy * (2 * normFirstDerivative / normVectorMult - accuracy / multiplicationFirstDerivative) );
}


//-----------------------------------------------------------------------------
/**
  Возвращает полилинию для геометрического примитива с точностью accuracy. Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
*/
//---
void Curve::GetAsPolyLine( GeomPolyline &polyLine, double accuracy ) const
{
  std::vector<Point> polyLinePoints;
  double t = GetRange().GetStart();
  polyLinePoints.push_back( GetPoint(t) );
  while ( t < GetRange().GetEnd() )
  {
    double current = CountingStep( t, accuracy);
	//if (current > 1000.0 * accuracy)
	//	current = (GetRange().GetEnd() - GetRange().GetStart()) / 100.0;
	//if (current < 0)
	//	current = (GetRange().GetEnd() - GetRange().GetStart()) / 100.0;
	t = t + current;
	Point point ( GetPoint(t) );
    polyLinePoints.push_back( point );
  }
   polyLine.Init( polyLinePoints );
}


//-----------------------------------------------------------------------------
/**
	Получить по параметру t, паремтр, врходящий в область определения для геоме6трического примитива. 
*/
//---
double Curve::FixedRange( double t ) const
{
  if ( t < GetRange().GetStart() )
    return GetRange().GetStart();
  if ( t > GetRange().GetEnd() )
    return GetRange().GetEnd();
  return t;
}
}
