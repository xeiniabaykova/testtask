#include "Intersector.h"
#include "Line.h"
#include "Ellipse.h"
#include "GeomPolyline.h"
#include <algorithm>
namespace Math {

std::vector<Point> IntersectPolylineCircle( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}

std::vector<Point> IntersectPolylineEllipse( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


int FindIntersection( double u0, double u1, double  v0, double v1, double* w )
{
  if( u1 < v0 || u0 > v1 )
    return 0;
  if ( u1 > v0 )
  {
    if ( u0 < v1 )
    {
      if (u0 < v0)
        w[0] = v0;
      else
        w[0] = u0;
      if ( u1 > v1 )
        w[1] = v1;
      else
        w[1] = u1;
      return 2;
    } else
    {
      w[0] = u0;
      return 1;
    }
  }
  else
  {
    w[0] = u1;
    return 1;
  }
}


std::vector<Point> IntersectLines( const Curve& curve1, const Curve& curve2 )
{
  std::vector<Point> IntersectPoints;
  const Math::Line* lineCurveFirst =  dynamic_cast<const Math::Line*>( &curve1 );
  const Math::Line* lineCurveSecound =  dynamic_cast<const Math::Line*>( &curve2 );

  const Point P0 = lineCurveFirst->GetStartPoint();
  const Vector V0 = lineCurveFirst->GetEndPoint() - lineCurveFirst->GetStartPoint();

  const Point P1 = lineCurveSecound->GetStartPoint();
  const Vector V1 = lineCurveSecound->GetEndPoint() - lineCurveSecound->GetStartPoint();

  Vector distanceStart = P1 - P0;
  float krossDirecting = V0.VectorMult( V1 );
  float sqrKross = krossDirecting * krossDirecting;
  float sqrLen0 = V0.Lenght();
  float sqrLen1 = V1.Lenght();
  if ( !V0.IsCollinear(V1) )
  {

    float s = distanceStart.VectorMult( V1 ) / krossDirecting;
    if( s < 0 || s > 1 )
    {
      return IntersectPoints;
    }
    float t = distanceStart.VectorMult( V0 ) / krossDirecting;
    if( t < 0 || t > 1)
    {
      return IntersectPoints;
    }
    IntersectPoints.push_back(Point( t, s ) );
    return IntersectPoints;

 } else
  {
    float sqrLenE = distanceStart.Lenght() * distanceStart.Lenght();
    double kross = distanceStart.VectorMult( V0 );
    double  sqrKross = kross * kross;
    if ( sqrKross > sqrLen0 * sqrLenE *CommonConstantsMath::NULL_TOL )
    {
      return IntersectPoints;
    }
    double  s0 = V0 * distanceStart / sqrLen0;
    double s1 = ( s0 + V0 * V1 )/sqrLen0;
    double w[2];
    double smin = std::min( s0, s1 );
    double smax = std::max( s0, s1 );
    int imax = FindIntersection( 0.0, 1.0, smin, smax, w );
    for ( int i = 0; i < imax; ++i )
      IntersectPoints.push_back( P0 + V0 * w[i] );
    return IntersectPoints;
  }

}

std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2 )
{
  std::vector<Point> intersectPoints;
  const Math::GeomPolyline* lineCurveFirst =  dynamic_cast<const Math::GeomPolyline*>( &curve1 );
  std::vector<Point> refPointsFirst = lineCurveFirst->GetReferensedPoints();
  const Math::GeomPolyline* lineCurveSecound =  dynamic_cast<const Math::GeomPolyline*>( &curve2 );
  std::vector<Point> refPointsSecound = lineCurveSecound->GetReferensedPoints();
  for ( int i = 1; i < refPointsFirst.size(); i++ )
  {
    Line firstLine( refPointsFirst[i -1], refPointsFirst[i]);
    for ( int j = 1; j < refPointsSecound.size(); j++ )
    {
     std::vector<Point> currentPoints = IntersectLines( Line(refPointsSecound[j - 1], refPointsSecound[j]), firstLine );	 
		 for (int k = 0; k < currentPoints.size(); k++)
		 {
       Point currentPoint( i + currentPoints[k].GetX() -1., j + currentPoints[k].GetY() -1.);
		   auto result = std::find( std::begin(intersectPoints), std::end(intersectPoints), currentPoint );
		   if ( result == std::end(intersectPoints) ) 
			 intersectPoints.push_back(currentPoint);
		 }
    }
  }
  return intersectPoints;
}

std::vector<Point> IntersectLinePolyline( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


std::vector<Point> IntersectLineEllipse( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}

std::vector<Point> IntersectLineCircle( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


std::vector<std::vector<double>> CountingHessian( const Curve& curve1, const Curve& curve2, double t1, double t2 )
{
  const double x1 = curve1.GetPoint(t1).GetX();
  const double y1 = curve1.GetPoint(t1).GetY();
  const double x1d = curve1.GetDerivative(t1).GetX();
  const double y1d = curve1.GetDerivative(t1).GetY();
  const double x1dd = curve1.Get2Derivative(t1).GetX();
  const double y1dd = curve1.Get2Derivative(t1).GetY();

  const double x2 = curve2.GetPoint(t2).GetX();
  const double y2 = curve2.GetPoint(t2).GetY();
  const double x2d = curve2.GetDerivative(t2).GetX();
  const double y2d = curve2.GetDerivative(t2).GetY();
  const double x2dd = curve2.Get2Derivative(t2).GetX();
  const double y2dd = curve2.Get2Derivative(t2).GetY();

  const double F11 = x1dd * 2. * (x1 - x2) + 2. * x1d * x1d + y1dd * 2. * (y1 - y2) + 2. * y1d * y1d;
  const double F12 = -2. * x2d * x1d - 2. * y2d * y1d;
  const double F21 = -2. *x1d * x2d - 2. * y1d * y2d;
  const double F22 = -2. * (x1 - x2) * x2dd + 2.* x2d * x2d - 2. * (y1 - y2) * y2dd + 2. * y2d * y2d;

  std::vector<std::vector <double>> hessian;
  hessian.resize(2);
  hessian[0].resize(2);
  hessian[1].resize(2);
  hessian[0][0] = F11;
  hessian[0][1] = F12;
  hessian[1][0] = F21;
  hessian[1][1] = F22;
  return hessian;

}
void InverseMatrix( std::vector<std::vector<double>>& matrix )
{
  const double d = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
  const double a00 = matrix[1][1] / d;
  const double a01 = -matrix[0][1] / d;
  const double a10 =  - matrix[1][0] / d;
  const double a11 = matrix[0][0] / d;
  matrix[0][0] = a00;
  matrix[0][1] = a01;
  matrix[1][0] = a10;
  matrix[1][1] = a11;
}
Point Gradient( const Curve& curve1, const Curve& curve2, Point start )
{
  Point aPoint1 = curve1.GetPoint(start.GetX());
  Vector aGrad1 = curve1.GetDerivative(start.GetX());

  Point aPoint2 = curve2.GetPoint( start.GetY() );
  Vector aGrad2 = curve2.GetDerivative(start.GetY());
  double aResultT1 = 2.0 * (aPoint1.GetX()- aPoint2.GetX()) * aGrad1.GetX()+ 2.0 * (aPoint1.GetY()- aPoint2.GetY()) * aGrad1.GetY();
  double aResultT2 = 2.0 * (aPoint2.GetX() - aPoint1.GetX()) * aGrad2.GetX() + 2.0 * (aPoint2.GetY()- aPoint1.GetY()) * aGrad2.GetY();
  return Point(aResultT1, aResultT2);
}
Point NewtonMethodIteration(const Curve& curve1, const Curve& curve2, Point startValue)
{
  std::vector<std::vector<double>> hessian = CountingHessian( curve1, curve2, startValue.GetX(), startValue.GetY() );
  InverseMatrix( hessian );
  Point grad = Gradient( curve1, curve2, startValue );
  Point temp(hessian[0][0] * -grad.GetX() + hessian[0][1] * -grad.GetY(), hessian[1][0] * -grad.GetX() + hessian[1][1] * -grad.GetY());

  return  startValue + temp;
}
Point NewtonMethod( const Curve& curve1, const Curve& curve2, Point startValue )
{
  int numiteration = 10;
  Point currentPoint = startValue;
  Point newPoint;
  for ( int i = 0; i < numiteration; i++ ) {
    newPoint = NewtonMethodIteration( curve1, curve2, currentPoint );
    if ( fabs(Distance(newPoint, currentPoint)) < CommonConstantsMath::NULL_TOL )
      break;
    currentPoint = newPoint;
  }
  return newPoint;
}


std::vector<Point> IntersectGeneralCase( const Curve& curve1, const Curve& curve2 )
{
	GeomPolyline polylinePoints1;
  std::vector<double> refcurve1;
	curve1.GetAsPolyLine(polylinePoints1, refcurve1);

	GeomPolyline polylinePoints2;
  std::vector<double> refcurve2;
	curve2.GetAsPolyLine(polylinePoints2, refcurve2);

	std::vector<Point> startPoints = IntersectPolylinePolyline( polylinePoints1, polylinePoints2 );
	std::vector<Point> intersectPoints;

	for (int i = 0; i < startPoints.size(); i++)
	{
    double paramcurve1;
    double paramcurve2;
    std::modf( startPoints[i].GetX(), &paramcurve1 );
    std::modf( startPoints[i].GetY(), &paramcurve2 );
    intersectPoints.push_back( NewtonMethod( curve1, curve2, Point(refcurve1[paramcurve1], refcurve2[paramcurve2])) );
	}
  return intersectPoints;
}


std::vector<Point> IntersectCurves( const Curve& curve1, const Curve& curve2 )
{
  std::vector<Point> intersectCurves;
  Curve::CurveType typeCurve1 = curve1.GetType();
  Curve::CurveType typeCurve2 = curve2.GetType();

  switch ( typeCurve1 )
  {
    case Curve::LineType :
      switch ( typeCurve2 )
      {
      case Curve::LineType :
        return IntersectLines( curve1, curve2 );
      case Curve::PolylineType :
        return IntersectLinePolyline( curve1, curve2 );
      case Curve::CircleType :
        return IntersectLineCircle( curve1, curve2 );
      case Curve::EllipseType :
        return IntersectLineEllipse( curve1, curve2);
      case Curve::NurbsType :
        return IntersectGeneralCase( curve1, curve2 );
      }
    case Curve::PolylineType :
      switch  ( typeCurve2 )
      {
        case Curve::PolylineType :
          return IntersectPolylinePolyline( curve1, curve2);
        case Curve::CircleType :
          return IntersectPolylineCircle( curve1, curve2 );
        case Curve::EllipseType :
          return IntersectPolylineEllipse( curve1, curve2 );
        case Curve::NurbsType :
          return IntersectGeneralCase( curve1, curve2 );
      }
    case Curve::CircleType :
      return IntersectGeneralCase( curve1, curve2 );
    case Curve::EllipseType :
      return IntersectGeneralCase( curve1, curve2 );
     case Curve::NurbsType :
      return IntersectGeneralCase( curve1, curve2 );
    break;

  }
}


}
