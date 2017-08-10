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
void Curve::GetAsPolyLine( GeomPolyline &polyLine,double accuracy) const
{
  std::vector<Point> polyLinePoints;

  double t = GetRange().GetStart();

  while ( t <= GetRange().GetEnd() )
  {
    polyLinePoints.push_back( GetPoint(t) );
    t += CountingStep( t, accuracy);
  }
  if (!IsEqual(polyLinePoints.back(), GetPoint(GetRange().GetEnd())))
  {
    polyLinePoints.push_back(GetPoint(GetRange().GetEnd()));
  }
   polyLine.Init( polyLinePoints );

}


//-----------------------------------------------------------------------------
//	Получить по параметру t, паремтр, врходящий в область определения для геоме6трического примитива.
// ---
void Curve::FixParameter( double& t ) const
{
  const Range range = GetRange();
  if ( IsClosed() )
  {   
    while ( t < range.GetStart() )
      t += range.Lenght();
    while ( t > range.GetEnd() )
      t -= range.Lenght();
  }
  else
  {
    if ( t < range.GetStart() )
      t = range.GetStart();
    if ( t > range.GetEnd() )
      t = range.GetEnd();
  }
}


//-----------------------------------------------------------------------------
//	Получить для заданной точности параметры использованные для построения полилинии.
// ---
void Curve::GetReferensedParams( std::vector<double>& referensedParams, double accuracy ) const
{
  double t = GetRange().GetStart();

  while ( t <= GetRange().GetEnd() )
  {
    referensedParams.push_back( t );
    t += CountingStep( t, accuracy );
  }
  if ( !IsEqual( GetPoint(referensedParams.back()), GetPoint(GetRange().GetEnd())) )
  {
    referensedParams.push_back( GetRange().GetEnd() );
  }
}
}
