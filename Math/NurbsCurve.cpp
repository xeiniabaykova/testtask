#include "NurbsCurve.h"
#include <cmath>

namespace Math {


NurbsCurve::NurbsCurve( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
  const std::vector<double>& nnodes, bool iisClosed, int ddegree ):
        poles(ppoles),
        weights(wweights),
        nodes(nnodes),
        isClosed(iisClosed),
        degree(ddegree)
{
if ( isClosed )
{
  for ( size_t i=0; i<degree; ++i )
  {
    weights.push_back( weights[i] );
    poles.push_back( poles[i] );
  }
}
}
void NurbsCurve::ComputeBasicFunction(double x, int i, double& result) const
{
  double p = degree;
  std::vector<double> N;
  N.resize(p + 1);

  if ((i == 0 && x == nodes[0]) ||
    (i == nodes.size() - p - 2 && x == nodes[nodes.size() - 1]))
  {
    result = 1.0;
    return;
  }
  if (x < nodes[i] || x >= nodes[i + p + 1])
  {
    result = 0.0;
    return;
  }
  for (int j = 0; j <= p; j++)
  {
    if (x >= nodes[i + j] && x < nodes[i + j + 1])
      N[j] = 1.0;
    else
      N[j] = 0.0;
  }
  for (int k = 1; k <= p; k++)
  {
    double saved = 0.0;
    if (N[0] == 0.0)
      saved = 0.0;
    else
      saved = ((x - nodes[i]) * N[0]) / (nodes[i + k] - nodes[i]);
    for (int j = 0; j < p - k + 1; j++)
    {
      double uLeft = nodes[i + j + 1];
      double uRight = nodes[i + j + k + 1];
      if (N[j + 1] == 0.0)
      {
        N[j] = saved;
        saved = 0.0;
      }
      else
      {
        double temp = N[j + 1] / (uRight - uLeft);
        N[j] = saved + (uRight - x)* temp;
        saved = (x - uLeft) * temp;
      }
    }
  }
  result = N[0];
}

int NurbsCurve::FindSpan( double x ) const
{
	int n = 0.0;
	int low = degree;
	n = nodes.size() - degree - 1;
	low = degree;

  if (x == nodes[n]) return n - 1;


  int hight = n + 1;
  int mid = (low + hight) / 2;
  while ((x < nodes[mid]) || (x >= nodes[mid + 1]))
  {
    if (x < nodes[mid])
      hight = mid;
    else
      low = mid;
    mid = (low + hight) / 2;
  }
  return mid;
}

void NurbsCurve::ComputeBasicFunctionD(double x, int i, double& result, int derivativeOrder) const
{
  if (x < nodes[i] || x >= nodes[i + degree + 1])
  {
    result = 0.0;
    return;
  }
  std::vector<std::vector<double>> vNodes;
  vNodes.resize(degree + 1);
  for (int j = 0; j < degree + 1; j++)
    vNodes[j].resize(degree + 1);

  for (int j = 0; j <= degree; j++)
  {
    if ((x >= nodes[i + j]) && (x < nodes[i + j + 1]))
      vNodes[j][0] = 1.0;
    else
      vNodes[j][0] = 0.0;
  }

  for (int k = 1; k <= degree; k++)
  {
    double saved;
    if (vNodes[0][k - 1] == 0.0)  saved = 0.0;
    else saved = ((x - nodes[i]) * vNodes[0][k - 1]) / (nodes[i + k] - nodes[i]);
    for (int j = 0; j < degree - k + 1; j++)
    {
      double uLeft = nodes[i + j + 1];
      double uRight = nodes[i + j + k + 1];
      if (vNodes[j + 1][k - 1] == 0.0)
      {
        vNodes[j][k] = saved;
        saved = 0.0;
      }
      else
      {
        double temp = vNodes[j + 1][k - 1] / (uRight - uLeft);
        vNodes[j][k] = saved + (uRight - x) * temp;
        saved = (x - uLeft) * temp;
      }
    }
  }
  std::vector<double> derivative;
  derivative.resize(degree);
  derivative[0] = vNodes[0][degree];
  for (int k = 1; k <= derivativeOrder; k++)
  {
    for (int j = 0; j <= k; j++)
      derivative[j] = vNodes[j][degree - k];

    double saved = 0.0;
    for (int m = 1; m <= k; m++)
    {
      if (derivative[0] == 0.0) saved = 0.0;
      else
        saved = derivative[0] / (nodes[i + degree - k + m] - nodes[i]);
      for (int j = 0; j < k - m + 1; j++)
      {
        double uLeft = nodes[i + j + 1];
        double uRight = nodes[i + j + degree + m + 1];
        if (derivative[j + 1] == 0.0)
        {
          derivative[j] = (degree - k + m) * saved;
          saved = 0.0;
        }
        else
        {
          double temp = derivative[j + 1] / (uRight - uLeft);
          derivative[j] = (degree - k + m) * (saved - temp);
          saved = temp;
        }
      }
    }
    result = derivative[0];
  }

}
double NurbsCurveBasicFunction()
{
  return 0.0;
}

void NurbsCurve::Translate(double xShift, double yShift)
{

}
void NurbsCurve::Rotate(double alpha)
{

}
void NurbsCurve::Scale(double XScaling, double YScaling)
{

}

double NurbsCurve::CountWeight(int k, double x)  const
{
  double w = 0.0;
  double n = 0;
  std::vector<double> node = BasicFunctions(k, x);

  /*if (isClosed)
  {
	  
	  for (int i = 0; i <= degree; i++)
	  {
		  int n = k - degree + i;
		  if (n >= (int)poles.size())
			  n = n - (poles.size());
		  if (n < 0)
			  n = n + (poles.size() );;
		  w = w + node[i] * weights[n];
	  }
	  return w;

  }*/
  for (int i = 0; i <= degree; i++)
  {
    w = w + node[i] * weights[k - degree -n + i];
  }
  return w;
}

double NurbsCurve::CountWeightD(double t)  const
{
  double w = 0.0;
  for (int i = 0; i < weights.size(); i++)
  {
    double result;
    ComputeBasicFunctionD(t, i, result, 1);
    w += result*weights[i];
  }
  return w;
}

double NurbsCurve::CountWeightD2(double t)  const
{
  double w = 0.0;
  for (int i = 0; i < weights.size(); i++)
  {
    double result;
    ComputeBasicFunctionD(t, i, result, 2);
    w += result * weights[i];
  }
  return w;
}


Point NurbsCurve::GetPoint(double t) const
{
	int n = 0;
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
		  n = span - degree - i;
		  if (n > (int)poles.size()) {
			  n = n - (poles.size());
		  }
		  if (n < 0) {
			  n = n + (poles.size());
		  }

		  resultPoint = resultPoint + poles[n] * node[i] * weights[n];
	  }
	  return Point(resultPoint *(1 / weightNurbs));

  for (int i = 0; i <= degree; i++)
  {
    resultPoint = resultPoint + poles[span - degree + i] * node[i] * weights[span - degree + i];
  }
  return Point(resultPoint *(1 / weightNurbs) );
}


std::vector<double> NurbsCurve::BasicFunctions( int i, double x) const
{
  std::vector<double> N;
    N.resize(degree + 1);
    std::vector<double> left;
    left.resize(degree + 1);
    std::vector<double> right;
    right.resize(degree + 1);
    N[0] = 1.0;
	/*if (isClosed)
	{
		std::vector<double> closedNodes;
		for (int i = degree; i > 0; i--)
			closedNodes.push_back(nodes[nodes.size() - degree - i]);
		for (int i = degree; i < nodes.size() - degree; i++)
			closedNodes.push_back(nodes[i]);
		for (int i = 0; i < degree; i++)
			closedNodes.push_back(nodes[degree + i]);
		for (int j = 1; j <= degree; j++)
		{
			left[j] = x - closedNodes[i + 1 - j];
			right[j] = closedNodes[i + j] - x;
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
	}*/
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


Vector  NurbsCurve::GetDerivativePoint(double t) const
{
  double span = FindSpan(t);
  double weightNurbsD = CountWeightD(t);
  Point resultPoint;
  for (int i = 0; i < poles.size(); i++)
  {
    double result;
    ComputeBasicFunctionD(t, span, result, 1);
    resultPoint = resultPoint + poles[i] * result * weights[i];
  }
  return Vector( ( resultPoint * ( 1 / weightNurbsD )).GetX(), (resultPoint * ( 1 / weightNurbsD)).GetY() );
}

Vector  NurbsCurve::Get2DerivativePoint(double t) const
{
  double span = FindSpan(t);
  double weightNurbsD2 = CountWeightD2(t);
  Point resultPoint;
  for (int i = 0; i < poles.size(); i++)
  {
    double result;
    ComputeBasicFunctionD(t, span, result, 2);
    resultPoint = resultPoint + poles[i] * result * weights[i];
  }
  return Vector( (resultPoint *(1 / weightNurbsD2)).GetX(), (resultPoint *(1 / weightNurbsD2)).GetY() );
}

Range  NurbsCurve::GetRange() const
{
	if (isClosed)
		return Range(nodes[degree], nodes[nodes.size() - 1 - degree]);

  return Range(nodes[0], nodes[nodes.size() - 1]);
}

void  NurbsCurve::GetAsPolyLine( std::vector<Point>& polyLinePoints, double accuracy ) const
{
  double t = GetRange().GetStart() + 0.1;
  while (t <= GetRange().GetEnd())
  {
    polyLinePoints.push_back( GetPoint(t) );
    t = t + 0.1;
  }
}


double NurbsCurve::DistanceToPoint(Point point) const
{

  double accuracy = 0.01;
  std::vector<Point> polylinePoints;
  GetAsPolyLine(polylinePoints, accuracy);
  return Curve::DistancePointToCurve( point, polylinePoints );
}

std::string NurbsCurve::GetName() const
{
  return "Nurbs";
}

bool NurbsCurve::IsValid() const
{
  return ( nodes.size() );
}

std::vector<Point>  NurbsCurve::GetPoles() const
{
  return poles;
}
std::vector<double> NurbsCurve::GetWeights() const
{
  return weights;
}
std::vector<double> NurbsCurve::GetNodes() const
{
  return nodes;
}
bool NurbsCurve::IsClosed() const
{
  return isClosed;
}
double NurbsCurve::Degree() const {
  return degree;
}
}
