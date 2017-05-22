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


void GeometricPoint::Translation(  double xShift, double yShift )
{
  point = point + Point( xShift, yShift );
}

void GeometricPoint::Rotation( double )
{
  return;
}

void GeometricPoint::Scaling(  double, double  )
{
  return;
}
