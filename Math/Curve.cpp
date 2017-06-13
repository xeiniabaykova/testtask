#include "Curve.h"
#include <cmath>
#include <limits>
#include "GeomPolyline.h"


namespace Math {
//-----------------------------------------------------------------------------
//  Расчитывается параметрический шаг по кривой, позволяющий апроксимировать кривую с необходимой точностью.
// ---
double Curve::CountingStep( double tCurrent, double accuracy) const
{
  const Vector firstDerivative = GetDerivative( tCurrent );
  const Vector secondDerivative = Get2Derivative( tCurrent );
  const double vectorMult = firstDerivative.VectorMult( secondDerivative );
  const double normVectorMult = sqrt( vectorMult * vectorMult );
  const double multiplicationFirstDerivative = firstDerivative * firstDerivative;
  const double normFirstDerivative = sqrt( multiplicationFirstDerivative );
  return 2 * sqrt ( accuracy * (2 * normFirstDerivative / normVectorMult - accuracy / multiplicationFirstDerivative) );
}


//-----------------------------------------------------------------------------
//  Возвращает полилинию для геометрического примитива с точностью accuracy.
// Точки, составляющие полилинию, расчитываются с помощью функции countingStep.
// ---
void Curve::GetAsPolyLine( GeomPolyline &polyLine, double accuracy ) const
{
  std::vector<Point> polyLinePoints;
  double t = GetRange().GetStart();

  while ( t <= GetRange().GetEnd() )
  {
    polyLinePoints.push_back( GetPoint(t) );
    t += CountingStep( t, accuracy);
  }
   polyLine.Init( polyLinePoints );
}


//-----------------------------------------------------------------------------
//	Получить по параметру t, паремтр, врходящий в область определения для геоме6трического примитива.
// ---
double Curve::FixParametr( double t ) const
{
  Range range = GetRange();
  if ( !IsClosed() )
  {
    if ( t < range.GetStart() )
      t = range.GetStart();
    if ( t > range.GetEnd() )
      t = range.GetEnd();
  } else
  {
    while ( t < range.GetStart() )
      t += range.Lenght();
    while ( t > range.GetEnd() )
      t -= range.Lenght();
  }
  return t;
}
}
