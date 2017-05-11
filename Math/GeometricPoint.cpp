#include "GeometricPoint.h"

//-----------------------------------------------------------------------------
/**
  \ru Коструктор GeometricPoint
*/
//-----------------------------------------------------------------------------
GeometricPoint::GeometricPoint( Point point ):
  point( point )
{
}


void GeometricPoint::Translation( const std::vector<std::vector<double>>& )
{
  return;
}

void GeometricPoint::Rotation( const std::vector<std::vector<double>>& )
{
  return;
}

void GeometricPoint::Dilatation( const std::vector<std::vector<double>>& )
{
  return;
}
