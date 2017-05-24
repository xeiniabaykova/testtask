#include "Line.h"
#include "CommonConstantsMath.h"
#include <cmath>

namespace {

bool IsEqualPoint(Point point1, Point point2)
{
  if ( fabs(point1.GetX() - point2.GetX()) < CommonConstantsMath::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstantsMath::NULL_TOL )
    return true;
  return false;
}
}



//-----------------------------------------------------------------------------
/**
  \ru Коструктор Line Записывается начальная точка и вектор направления
*/
//-----------------------------------------------------------------------------
Line::Line( Point startPoint, Point endPoint ):
  C2Curve(),
  startPoint( startPoint ),
  endPoint  ( endPoint )
{
  if ( !IsEqualPoint(startPoint, endPoint) )
	{
		std::vector<Point> points;
		points.push_back(startPoint);
		points.push_back(endPoint);
		SetReferensedPoints(points);
	}
}

Line::Line( const std::vector<Point>& points ):
  C2Curve(),
  startPoint( Point(0.0, 0.0) ),
  endPoint  ( Point(0.0, 0.0) )
{
  if ( points.size() >= 2 && !IsEqualPoint(points[0], points[1]) )
  {
    startPoint = points[0];
    endPoint = points[1];
    SetReferensedPoints( points );
  }
}

//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для прямой: [0, 1]
*/
//-----------------------------------------------------------------------------
Range Line::GetRange() const
{
  return Range( 0.0, 1.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point Line::GetPoint( double t ) const
{
  Point direction( endPoint.GetX() - startPoint.GetX(), endPoint.GetY() - startPoint.GetY());
  return Point( startPoint.GetX() + direction.GetX() * t, startPoint.GetY() + direction.GetY() * t );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Vector Line::GetDerivativePoint( double ) const
{
  Vector direction( endPoint.GetX() - startPoint.GetX(), endPoint.GetY() - startPoint.GetY());
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Vector Line::Get2DerivativePoint( double ) const
{
  return Vector( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для текущей линии - это начало конец
*/
//-----------------------------------------------------------------------------
void Line::GetAsPolyLine( std::vector<Point> & polyLinePoints, double ) const
{
  polyLinePoints.clear();
  polyLinePoints.push_back( startPoint );
  polyLinePoints.push_back( endPoint );
}

void Line::Translate( double xShift, double yShift )
{
  startPoint = Point( startPoint.GetX() + xShift, startPoint.GetY() + yShift );
  endPoint = Point( endPoint.GetX() + xShift, endPoint.GetY() + yShift );
}

void Line::Rotate( double alpha )
{
  double cosAlpha = cos( alpha );
  double sinAlpha = sin( alpha );
  startPoint = Point( startPoint.GetX() * cosAlpha - startPoint.GetY() *sinAlpha,
                      startPoint.GetX() * sinAlpha + startPoint.GetY() * cosAlpha );

  endPoint = Point( endPoint.GetX() * cosAlpha - endPoint.GetY() * sinAlpha,
                      endPoint.GetX() * sinAlpha + endPoint.GetY() * cosAlpha );
}

void Line::Scale( double xScaling, double yScaling )
{
  startPoint = Point( startPoint.GetX() * xScaling, startPoint.GetY() * yScaling );
  endPoint = Point( endPoint.GetX() * xScaling, endPoint.GetY() * yScaling );
}

double Line::DistanceToPoint( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, CommonConstantsMath::PRECISION_POLYLINE );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}

std::string Line::GetName() const
{
  return "Line";
}


bool Line::IsValid() const
{
  return ( !IsEqualPoint(startPoint, endPoint) );
}
