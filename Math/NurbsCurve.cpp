#include "NurbsCurve.h"
#include <cmath>

namespace Math {


NurbsCurve::NurbsCurve( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
  const std::vector<double>& nnodes, bool iisClosed, int ddegree ):
  poles    ( ppoles ),
  weights  ( wweights ),
  nodes    ( nnodes ),
  isClosed ( iisClosed ),
  degree   ( ddegree )
{

  if ( nnodes.size() == 0 ) 
  { 
	  for (size_t i = 0; i < degree + 1; ++i) {
		  nodes.push_back(0.);
	  }
	  double node = 1.;
	  for ( size_t i = 0; i < ppoles.size() - degree - 1; ++i ) {
		  nodes.push_back(node);
		  node += 1.;
	  }
	  for ( size_t i = 0; i < degree + 1; ++i ) {
		  nodes.push_back(node);
	  }
  }
  if ( isClosed )
  {
    for ( size_t i=0; i<degree; ++i )
    {
      weights.push_back( weights[i] );
      poles.push_back( poles[i] );
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Подсчитать базисную функцию для порядка i и значения парметра t.
*/
//---
void NurbsCurve::ComputeBasicFunction( double x, int i, double& result ) const
{
  double p = degree;
  std::vector<double> N;
  N.resize( p + 1 );

  if ( (i == 0 && x == nodes[0]) ||
    (i == nodes.size() - p - 2 && x == nodes[nodes.size() - 1]) )
  {
    result = 1.0;
    return;
  }
  if ( x < nodes[i] || x >= nodes[i + p + 1] )
  {
    result = 0.0;
    return;
  }
  for ( int j = 0; j <= p; j++ )
  {
    if ( x >= nodes[i + j] && x < nodes[i + j + 1] )
      N[j] = 1.0;
    else
      N[j] = 0.0;
  }
  for (int k = 1; k <= p; k++)
  {
    double saved = 0.0;
    if ( N[0] == 0.0 )
      saved = 0.0;
    else
      saved = ( (x - nodes[i]) * N[0]) / (nodes[i + k] - nodes[i] );
    for ( int j = 0; j < p - k + 1; j++ )
    {
      double uLeft = nodes[i + j + 1];
      double uRight = nodes[i + j + k + 1];
      if ( N[j + 1] == 0.0 )
      {
        N[j] = saved;
        saved = 0.0;
      }
      else
      {
        double temp = N[j + 1] / ( uRight - uLeft );
        N[j] = saved + ( uRight - x ) * temp;
        saved = ( x - uLeft ) * temp;
      }
    }
  }
  result = N[0];
}


//-----------------------------------------------------------------------------
/**
  Найти интервал ненулевых базисных функций для парметра t.
*/
//---
int NurbsCurve::FindSpan( double x ) const
{
	int n = 0.0;
	int low = degree;
	n = nodes.size() - degree - 1;
	low = degree;

  if ( x == nodes[n] ) return n - 1;

  int hight = n + 1;
  int mid = ( low + hight ) / 2;
  while ( (x < nodes[mid]) || (x >= nodes[mid + 1]) )
  {
    if ( x < nodes[mid] )
      hight = mid;
    else
      low = mid;
    mid = ( low + hight ) / 2;
  }
  return mid;
}


//-----------------------------------------------------------------------------
/**
  Подсчитать производную от базисной функцию для порядка i и значения парметра t, где derivativeOrder - порядок производной.
*/
//---
void NurbsCurve::ComputeBasicFunctionD( double x, int i, int derivativeOrder, std::vector<std::vector<double>>& ders) const
{

	std::vector<std::vector<double>> tempders;
	tempders.resize(degree + 1);
	for (int k = 0; k < degree + 1; k++)
		tempders[k].resize(degree + 1);
	
	ders.resize(degree + 1);
	for (int k = 0; k < degree + 1; k++)
		ders[k].resize(degree + 1);

	std::vector<std::vector<double>> N;
	N.resize(degree + 1);
	for (int k = 0; k < degree + 1; k++)
		N[k].resize(degree + 1);
	std::vector<double> left;
	left.resize(degree + 1);
	std::vector<double> right;
	right.resize(degree + 1);
	N[0][0] = 1.0;
	for (int j = 1; j <= degree; j++)
	{
		left[j] = x - nodes[i + 1 - j];
		right[j] = nodes[i + j] - x;
		double saved = 0.0;
		for (int r = 0; r < j; r++)
		{
			N[j][r] = right[r + 1] + left[j - r];
			double temp = N[r][j - 1] / N[j][r];
			N[r][j] = saved + right[r + 1] * temp;
			saved = left[j - r] * temp;
		}
		N[j][j] = saved;
	}
	for (int j = 0; j <= degree; j++)
		ders[0][j] = N[j][degree];
	for (int r = 0; r <= degree; r++)
	{
	int s1 = 0;
		int s2 = 1;
		tempders[0][0] = 1.0;
		for (int k = 1; k <= derivativeOrder; k++)
		{
			double d = 0.0;
			int rk = r - k;
			int degk = degree - k;
			if (r >= k)
			{
				tempders[s2][0] = tempders[s1][0] / N[degk + 1][rk];
				d = tempders[s2][0] * N[rk][degk];
			}
			int j1 = 0;
			if (rk >= -1)
				j1 = 1;
			else
				j1 = -rk;
			int j2 = 0;
			if (r - 1 <= degk)
				j2 = k - 1;
			else 
				j2 = degree - r;

			for (int j = j1; j <= j2; j++)
			{
				tempders[s2][j] = (tempders[s1][j] - tempders[s1][j - 1]) /
					N[degk + 1][rk + j];
				d += tempders[s2][j] * N[rk + j][degk];
			}
			if (r <= degk)
			{
				tempders[s2][k] = -tempders[s1][k - 1] / N[degk + 1][r];
				d += -tempders[s2][k] * N[r][degk];
			}
			ders[k][r] = d;
			int j = s1;
			s1 = s2;
			s2 = j;
		}

	}
	int r = degree;
	for (int k = 1; k <= derivativeOrder; k++)
	{
		for (int j = 0; j <= degree; j++)
			ders[k][j] *= r;
		r *= (degree - k);
	}

	
}


//-----------------------------------------------------------------------------
/**
  Сдвиг по оси x на xShift, по оси y на yShift.
*/
//---
void NurbsCurve::Translate( double xShift, double yShift )
{
  for ( int i = 0; i < poles.size(); i++ )
    poles[i].Translate( xShift, yShift );

}


//-----------------------------------------------------------------------------
/**
  Повернуть относительно начала координат на угол alpha.
*/
//---
void NurbsCurve::Rotate( double alpha )
{
  for ( int i = 0; i < poles.size(); i++ )
    poles[i].Rotate( alpha );
}


//-----------------------------------------------------------------------------
/**
  Повернуть относительно начала координат на угол alpha.
*/
//---
void NurbsCurve::Scale(double XScaling, double YScaling)
{
  for ( int i = 0; i < poles.size(); i++ )
    poles[i].Scale( XScaling, YScaling );
}



//-----------------------------------------------------------------------------
/**
  Подсчитать значения базисных функций, умноженных на вес на отрезке x - degree.
*/
//---
double NurbsCurve::CountWeight( int k, double x )  const
{
  double w = 0.0;
  std::vector<double> node = BasicFunctions( k, x );

  for (int i = 0; i <= degree; i++)
  {
    w = w + node[i] * weights[k - degree  + i];
  }
  return w;
}


//-----------------------------------------------------------------------------
/**
  Подсчитать значения производных базисных функций, умноженных на вес на отрезке x - degree.
*/
//---
double NurbsCurve::CountWeightD( double t , int span)  const
{
  double w = 0.0;
  std::vector<std::vector<double>> ders;
  ComputeBasicFunctionD(t, span, 1, ders);
  for (int i = 0; i <=degree; i++)
  {
   
    w += ders[0][i] * weights[span - degree + i];
  }
  return w;
}


//-----------------------------------------------------------------------------
/**
  Подсчитать значения вторых производных базисных функций, умноженных на вес на отрезке x - degree.
*/
//---
double NurbsCurve::CountWeightD2( double t , int span)  const
{
  double w = 0.0;
  std::vector<std::vector<double>> ders;
  ComputeBasicFunctionD(t, span, 2, ders);
  for (int i = 0; i < weights.size(); i++)
  {
    double result;
    
    w += ders[1][i] * weights[i];
  }
  return w;
}


//-----------------------------------------------------------------------------
/**
   Вернуть точку на кривой по параметру t.
*/
//---
Point NurbsCurve::GetPoint( double t ) const
{
	if (!isClosed)
	{
		if (t < nodes[0])
			t = nodes[0];
		if (t > nodes[nodes.size() - 1])
			t = nodes[nodes.size() - 1];
	}
	if (isClosed)
	{
		Range range = GetRange();

		while (t < range.GetStart())
			t += range.GetEnd() - range.GetStart();
		while (t > range.GetEnd())
			t -= range.GetEnd() - range.GetStart();
	}
	int span = FindSpan(t);
	double weightNurbs = CountWeight(span, t);
	Point resultPoint(0.0, 0.0);
	std::vector<double> node = BasicFunctions(span, t);

	for (int i = 0; i <= degree; i++)
	{
		resultPoint = resultPoint + poles[span - degree + i] * node[i] * weights[span - degree + i];
	}
	return Point(resultPoint * (1 / weightNurbs));
}



//-----------------------------------------------------------------------------
/**
   Подсчитать значения базисных функций на отрезке x - degree.
*/
//---
std::vector<double> NurbsCurve::BasicFunctions( int i, double x) const
{
  std::vector<double> N;
  N.resize( degree + 1 );
  std::vector<double> left;
  left.resize( degree + 1 );
  std::vector<double> right;
  right.resize( degree + 1 );
  N[0] = 1.0;
  for (int j = 1; j <= degree; j++)
  {
    left[j] = x - nodes[i + 1 - j];
    right[j] = nodes[i + j] - x;
    double saved = 0.0;
    for (int k = 0; k < j; k++)
    {
      double temp = N[k] / (right[k + 1] + left[j - k]);
      N[k] = saved + right[k + 1] * temp;
      saved = left[j - k] * temp;
    }
    N[j] = saved;
  }
  return N;
}



//-----------------------------------------------------------------------------
/**
  Вернуть производную на nurbs по параметру t.
*/
//---
Vector  NurbsCurve::GetDerivativePoint( double t ) const
{
  double tcurrent = FixedRange(t);
  double span = FindSpan(tcurrent);
  double weightNurbsD = CountWeightD(tcurrent, span);
  std::vector<std::vector<double>> ders;
  ComputeBasicFunctionD( tcurrent, span, 1, ders );
  Point resultPoint;
  for (int i = 0; i <= degree; i++)
  {
    double result;
   
    resultPoint = resultPoint + poles[i] * ders[1][i] * weights[i];
  }
  return Vector( ( resultPoint * ( 1 / weightNurbsD )).GetX(), (resultPoint * ( 1 / weightNurbsD)).GetY() );
}


//-----------------------------------------------------------------------------
/**
  Вернуть вторую производную на nurbs по параметру t.
*/
//---
Vector  NurbsCurve::Get2DerivativePoint( double t ) const
{
	double tcurrent = FixedRange(t);
  double span = FindSpan(tcurrent);
  double weightNurbsD2 = CountWeightD2(tcurrent, span );
  Point resultPoint;
  std::vector<std::vector<double>> ders;
  ComputeBasicFunctionD(tcurrent, span, 2, ders);
  for ( int i = 0; i < poles.size(); i++ )
  {
    resultPoint = resultPoint + poles[i] * ders[2][i] * weights[i];
  }
  return Vector( (resultPoint *(1 / weightNurbsD2)).GetX(), (resultPoint *(1 / weightNurbsD2)).GetY() );
}



//-----------------------------------------------------------------------------
/**
  Вернуть границы параметра для Nurbs : [nodes[0], nodes[size]].
*/
//---
Range  NurbsCurve::GetRange() const
{
	if (isClosed)
		return Range(nodes[degree], nodes[nodes.size() - 1 - degree]);

  return Range(nodes[0], nodes[nodes.size() - 1]);
}


//-----------------------------------------------------------------------------
/**
  Вернуть полилинию для nurbs.
*/
//---
void  NurbsCurve::GetAsPolyLine( std::vector<Point>& polyLinePoints, double accuracy ) const
{
  double t = GetRange().GetStart() + 0.1;
  while (t <= GetRange().GetEnd())
  {
    polyLinePoints.push_back( GetPoint(t) );
    t = t + 0.1;
  }
}


//-----------------------------------------------------------------------------
/**
  Вернуть имя, используемое для записи примитива в файл.
*/
//---
std::string NurbsCurve::GetName() const
{
  return "Nurbs";
}


//-----------------------------------------------------------------------------
/**
  Проверка на правильность кривой.
*/
//---
bool NurbsCurve::IsValid() const
{
  return ( nodes.size() );
}


//-----------------------------------------------------------------------------
/**
  Вернуть опорные точки.
*/
//---
std::vector<Point> NurbsCurve::GetPoles() const
{
  return poles;
}



//-----------------------------------------------------------------------------
/**
  Вернуть веса опорных точек.
*/
//---
std::vector<double> NurbsCurve::GetWeights() const
{
  return weights;
}


//-----------------------------------------------------------------------------
/**
  Вернуть границы параметра t для базисных полиномов.
*/
//---
std::vector<double> NurbsCurve::GetNodes() const
{
  return nodes;
}


//-----------------------------------------------------------------------------
/**
  Вернуть замкнутость Nurbs - кривой.
*/
//---
bool NurbsCurve::IsClosed() const
{
  return isClosed;
}


//-----------------------------------------------------------------------------
/**
  Вернуть степень Nurbs - кривой.
*/
//---
double NurbsCurve::Degree() const
{
  return degree;
}

//-----------------------------------------------------------------------------
/**
  Вернуть опорные точки, использованные для построения nurbs - кривой.
*/
//---
std::vector<Point> NurbsCurve::GetReferensedPoints () const
{
  return poles;
}


}
