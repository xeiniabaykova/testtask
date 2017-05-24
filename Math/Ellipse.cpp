#include "Ellipse.h"
#include <vector>
#include <cmath>
#include "CommonConstantsMath.h"



namespace {



bool IsEqualPoint( Point point1, Point point2 ) 
{
  if ( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL )
		return true;
	return false;
}
bool IsEqualPoint(Point point1, Point point2, Point point3 )
{
	if (fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
		&& fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL
		&& fabs(point2.GetX() - point3.GetX()) < CommonConstantsMath::NULL_TOL
		&& fabs(point2.GetY() - point3.GetY()) < CommonConstantsMath::NULL_TOL)
		return true;
	return false;
}

double Distance(Point point1, Point point2)
{
	return sqrt((point1.GetX() - point2.GetX()) * (point1.GetX() - point2.GetX()) +
		(point1.GetY() - point2.GetY()) * (point1.GetY() - point2.GetY()));
}

double DistanceCircleToPoint(Point center, double r, Point point)
{
	if (Distance(point, center) < r) 
		return r - Distance(point, center);
	else 
		return Distance(point, center) - r;
}
bool IsCirclePoints( Point point1, Point point2, Point point3 )
{
	if (IsEqualPoint(point1, point2, point3))
		return false;

	if ( fabs(Distance(point1, point2) - Distance(point1, point3)) < CommonConstantsMath::NULL_TOL ) 
		return true;
	return false;
}

void FindRadiusCenter(Point point1, Point point2, Point point3, double& radius, Point& center )
{ 
	if ((Distance(point1, point2) - Distance(point2, point3)) < CommonConstantsMath::NULL_TOL)
	{
		radius = Distance(point1, point2);
		center = point2;
	}
	else if ((Distance(point2, point3) - Distance(point3, point1)) < CommonConstantsMath::NULL_TOL)
	{
		radius = Distance(point2, point3);
		center = point3;

	}
	else if ((Distance(point2, point1) - Distance(point1, point3)) < CommonConstantsMath::NULL_TOL)
	{
		radius = Distance(point2, point1);
		center = point1;
	}

}
bool PointsOneLine( Point point1, Point point2, Point point3 )
{
	if ( fabs((point3.GetX() - point1.GetX()) /
    (point2.GetX() - point1.GetX()) - (point3.GetY() - point1.GetY()) / (point2.GetY() - point1.GetY())) < CommonConstantsMath::NULL_TOL)
		return true;
	return false;
}

// данные в эллипсе правильные, если: точки не совпадают, не лежат на одной
bool CorrectEllipseData( Point point1, Point point2, Point point3 )
{
	if ( IsEqualPoint(point1, point2) ||
		IsEqualPoint(point2, point3) ||
		IsEqualPoint(point1, point3) )
		return false;
	if (PointsOneLine(point1, point2, point3))
		return false;
	return true;
}
// если обе точки не совпадают, то считаем, что все верно
bool CorrectCircleData( Point point1, Point point2 )
{
	if (IsEqualPoint(point1, point2))
		return false;
	return true;
}
}


//-----------------------------------------------------------------------------
/**
  \ru Коструктор Ellipse. Вычисляются два радиуса и записывается центральная точка
*/
//-----------------------------------------------------------------------------
Ellipse::Ellipse( Point center, double r1, double r2, double alpha ):
  C2Curve(),
  center( center ),
  r1    ( r1     ),
  r2    ( r2     ),
  alpha ( alpha  )
{
  std::vector<Point> points;
  points.push_back( center );
  SetReferensedPoints( points );
}


Ellipse::Ellipse ( const std::vector<Point>& points ):
  C2Curve(),
  center( Point(0.0,0.0) ),
  r1    ( 0.0   ),
  r2    ( 0.0    ),
  alpha ( 0.0  )
{  
	// если точки 2, то это - окружность, создаем окружность
  if ( points.size() == 2 )
  {
    if ( CorrectCircleData(points[0], points[1]) )
    {
		center = points[0];
    Point pointV( points[1] );
		double x = pointV.GetX() - center.GetX();
		double y = pointV.GetY() - center.GetY();
		double r = sqrt(x * x + y * y);
		r1 = r;
		r2 = r;
		alpha = 0;
    }
  }
  else if ( points.size() >= 3 )
  {
    if ( CorrectEllipseData(points[0], points[1], points[2]) )
	  {
		  center = points[0];
      Point pointV( points[1] );
		  double x = pointV.GetX() - center.GetX();
		  double y = pointV.GetY() - center.GetY();
      double axisA = sqrt( x * x + y * y );
      alpha = atan2( y, x );
		  r1 = axisA;
      Point newCoordPoint( points[2].GetX() - center.GetX(), points[2].GetY() - center.GetY() );
      Point point2( newCoordPoint.GetX() * cos(alpha) + newCoordPoint.GetY() * sin(alpha),
        -newCoordPoint.GetX() * sin(alpha) + newCoordPoint.GetY() * cos(alpha) );

      double axisB = ( sqrt(fabs((point2.GetY()) * (point2.GetY()) /
        (1 - (point2.GetX()) * (point2.GetX()) / (r1 * r1)))) );
		  r2 = axisB;
	  }
    else if ( IsCirclePoints(points[0], points[1], points[2]) )
	  {
		  double r = Distance(points[0], points[1]);
		  center = points[0];
		  alpha = 0;
		  r1 = r2 = r;
	  }
  
  }
}

//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Ellipse::GetPoint( double t ) const
{

   Point point( r1 * cos(t), r2 * sin(t) );
   Point transformPoint( center.GetX() + point.GetX() * cos(alpha) - point.GetY() * sin(alpha),
                        center.GetY() + point.GetX() * sin(alpha)   + point.GetY() * cos(alpha) );
   return transformPoint;
}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для эллипса
*/
//-----------------------------------------------------------------------------


Range Ellipse::GetRange() const
{
  return Range( 0.0, 2.0 * CommonConstantsMath::PI );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную элипса по параметру t
*/
//-----------------------------------------------------------------------------
Vector Ellipse::GetDerivativePoint( double t ) const
{
  Point point( r1 * -sin(t), r2 * cos(t) );

  Vector transformPoint( point.GetX() * cos(alpha) - point.GetY() * sin(alpha),
	  point.GetX() * sin(alpha) + point.GetY() * cos(alpha));

  return transformPoint;

}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую эллипса по параметру t
*/
//-----------------------------------------------------------------------------
Vector Ellipse::Get2DerivativePoint( double t ) const
{
  Point point( -r1 * cos(t), -r2 * sin(t) );

  Vector transformPoint(point.GetX() * cos(alpha) - point.GetY() * sin(alpha),
	  point.GetX() * sin(alpha) + point.GetY() * cos(alpha));

  return transformPoint;
}

double Ellipse::DistanceToPoint( Point point ) const
{
  std::vector<Point> polylinePoints;
  if (abs(r1 - r2) < CommonConstantsMath::NULL_TOL)
	  return DistanceCircleToPoint(center, r1, point);
  GetAsPolyLine( polylinePoints, CommonConstantsMath::PRECISION_POLYLINE );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}

void Ellipse::Translate( double xShift, double yShift )
{
  center = Point( center.GetX() + xShift, center.GetY() + yShift );
}

void Ellipse::Rotate( double alphaAng )
{
  alpha += alphaAng;
}

void Ellipse::Scale( double XScaling, double YScaling )
{
	r1 *= XScaling;
	r2 *= YScaling;
}

bool Ellipse::IsValid() const
{
	if (r1 < CommonConstantsMath::NULL_TOL || r2 < CommonConstantsMath::NULL_TOL)
		return false;
	return true;
}

std::string Ellipse::GetName() const
{
  return "Ellipse";
}
