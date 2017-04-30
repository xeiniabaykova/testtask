#include <Math/GeomPolyline.h>

//-----------------------------------------------------------------------------
/**
  Конструктор полилинии
*/
//-----------------------------------------------------------------------------
GeomPolyline::GeomPolyline(const std::vector<Point>& referensedPoits ):
  referencedPoints( referensedPoits ){}


//-----------------------------------------------------------------------------
/**
  \ru возращаются границы параметра t для полинии: [0, количество точек]
*/
//-----------------------------------------------------------------------------
Range GeomPolyline::GetRange() const
{
  return Range( 0.0, referencedPoints.size() );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращается точка по параметру t
*/
//-----------------------------------------------------------------------------
Point GeomPolyline::GetPoint( double t ) const
{
  int number = t;
  Point startPoint( referencedPoints[t] );
  Point derection( referencedPoints[t+1].GetX() - referencedPoints[t].GetX(),
      referencedPoints[t+1].GetY() - referencedPoints[t].GetY() );
  return Point( startPoint.GetX() + derection.GetX() * t, startPoint.GetY() + derection.GetY() * t );
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает производную полилиннии по параметру t
*/
//-----------------------------------------------------------------------------
Point GeomPolyline::GetDerivativePoint( double t ) const
{
  (void)t;
  int number = t;
  Point direction( referencedPoints[t+1].GetX() - referencedPoints[t].GetX(),
      referencedPoints[t+1].GetY() - referencedPoints[t].GetY() );
  return direction;
}


//-----------------------------------------------------------------------------
/**
  \ru возвращает вторую производную линнии по параметру t
*/
//-----------------------------------------------------------------------------
Point  GeomPolyline::Get2DerivativePoint( double t ) const
{
  (void)t;
  return Point ( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  \ru позвращается полилилния для полилинии - это полилиния
*/
//-----------------------------------------------------------------------------
void GeomPolyline::GetAsPolyLine( std::vector<Point> & polyLinePoints, double accuracy ) const
{
  polyLinePoints = referencedPoints;
}
