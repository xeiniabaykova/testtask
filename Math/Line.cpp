#include "Line.h"
#include "Editor/CommonConstants.h"
#include <cmath>

namespace {

bool IsEqualPoint(Point point1, Point point2)
{
  if (fabs(point1.GetX() - point2.GetX()) < CommonConstants::NULL_TOL
    && fabs(point1.GetY() - point2.GetY()) < CommonConstants::NULL_TOL )
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
  startPoint( startPoint ),
  endPoint( endPoint )
{
  std::vector<Point> points;
  points.push_back( startPoint );
  points.push_back( endPoint );
  SetReferensedPoints( points );
}

Line::Line( const std::vector<Point>& points )
{
  isValid = false;
  if (points.size() == 0)
    return;
  if ( IsEqualPoint(points[0], points[1]) )
    return;
  startPoint = points[0];
  endPoint = points[1];
  SetReferensedPoints( points );
  isValid = true;
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
Point Line::GetDerivativePoint( double ) const
{
  Point direction( endPoint.GetX() - startPoint.GetX(), endPoint.GetY() - startPoint.GetY());
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Point Line::Get2DerivativePoint( double ) const
{
  return Point ( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для текущей линии - это начало конец
*/
//-----------------------------------------------------------------------------
void Line::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  polyLinePoints.push_back( startPoint );
  polyLinePoints.push_back( endPoint );
}
void Line::Translation( double xShift, double yShift )
{
  startPoint = Point( startPoint.GetX() + xShift, startPoint.GetY() + yShift );
  endPoint = Point( endPoint.GetX() + xShift, endPoint.GetY() + yShift );
}

void Line::Rotation( double alpha )
{
  startPoint = Point( startPoint.GetX() * cos(alpha) - startPoint.GetY() * sin(alpha),
                      startPoint.GetX() * sin(alpha) + startPoint.GetY() * cos(alpha) );
  endPoint = Point( endPoint.GetX() * cos(alpha) - endPoint.GetY() * sin(alpha),
                      endPoint.GetX() * sin(alpha) + endPoint.GetY() * cos(alpha) );
}

void Line::Dilatation( double xScaling, double yScaling )
{
  startPoint = Point( startPoint.GetX() * xScaling, startPoint.GetY() * yScaling );
  endPoint = Point( endPoint.GetX() * xScaling, endPoint.GetY() * yScaling );
}

double Line::DistanceToPoint( Point point ) const
{
  std::vector<Point> polylinePoints;
  GetAsPolyLine( polylinePoints, CommonConstants::PRECISION_POLYLINE );
  return C2Curve::DistancePointToCurve( point, polylinePoints );
}

