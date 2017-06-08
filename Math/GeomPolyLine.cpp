#include "GeomPolyline.h"
#include <cmath>
#include <limits>

namespace Math {
namespace {

//-----------------------------------------------------------------------------
/**
  Полилиния считается верной, если количество точек не равно нулю, если точки не совпадают ( кроме траничных - полилиния может быть замкнутой).
*/
//---
static bool CorrectPolylineData( const std::vector<Point>& points )
{
  if ( points.size() < 2 )
    return false;
  if ( points.size() == 2 && IsEqual(points[0], points[1]) )
	  return false;
  for ( int i = 0; i < points.size() - 1; i++ )
    for ( int j = 0; j < points.size() - 1; j++ )
      {
        if ( i == j )
            continue;
         if ( IsEqual(points[i], points[j]) )
            return false;
       }
  return true;
}

//-----------------------------------------------------------------------------
/**
   Расстояние от точки до прямой.
   QPoint first, QPoint second - начальные и конечные точки прямой
   point - точка, до которой находится расстояние
*/
//-----------------------------------------------------------------------------
static double Distance( Point first, Point second, Point point )
{
   Vector v = second - first;
   Vector w = point - first;

   double c1 = v * w;
   if ( c1 <= 0 )
       return Distance( point, first );

   double c2 = v * v;
   if ( c2 <= c1 )
       return Distance( point, second );

   double b = c1 / c2;
   Point Pb = first + v * b;
   return Distance( point, Pb );
}

}


///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления полилинии.
/**
  Представлет функции для хранения и проведения опраций над полилинией.
*/
///////////////////////////////////////////////////////////////////////////////

GeomPolyline::GeomPolyline():
  referencedPoints( 0 )
{
}

//-----------------------------------------------------------------------------
/**
  Конструктор полилинии по опорным точкам. Если точки удовлетворяют условию корректности: их количество не равно нулю и нет совпадающих точек,
  то создается полилиния по воходым точкам. Считаем, что точки в полилинию добаляются в том же порядке, что и находятся в входном массиве.
*/
//---
GeomPolyline::GeomPolyline( const std::vector<Point>& thePoints ):
  referencedPoints( 0 )
{
  if ( CorrectPolylineData(thePoints) )
  {
    referencedPoints = thePoints;
  }
}

//-----------------------------------------------------------------------------
/**
  Инициализация полилинии по опорным точкам. Если точки удовлетворяют условию корректности: их количество не равно нулю и нет совпадающих точек,
  то создается полилиния по воходым точкам. Считаем, что точки в полилинию добаляются в том же порядке, что и находятся в входном массиве.
*/
//---
void GeomPolyline::Init( const std::vector<Point>& theReferencedPoints )
{
	referencedPoints.clear();
  if ( CorrectPolylineData(theReferencedPoints) )
  {
	  for ( int i=0; i<theReferencedPoints.size(); i++)
    referencedPoints.push_back( theReferencedPoints[i] );
  }
}

//-----------------------------------------------------------------------------
/**
 Вернуть границы параметра t для полинии: [0, количество точек].
*/
//---
Range GeomPolyline::GetRange() const
{
	if (IsValid())
	{
		return Range(0.0, referencedPoints.size() - 1);
	}
	else
		return Range(NAN,NAN);
}


//-----------------------------------------------------------------------------
/**
  Вернуть точку по параметру t.
*/
//---
Point GeomPolyline::GetPoint( double t ) const
{
	if ( IsValid() ) 
	{
		double tcurrent = FixedRange(t);
		double leftParam = 0.0;
    double currentT = std::modf( tcurrent, &leftParam );
    if ( leftParam >= referencedPoints.size() - 1 )
		{
      return  Point( referencedPoints[referencedPoints.size() - 1] );
		}
		Point startPoint( referencedPoints[leftParam] );
		Vector derection = referencedPoints[leftParam + 1] - referencedPoints[leftParam];
		return startPoint + derection * currentT;
	}
	else
		return Point( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Вернуть производную полилиннии по параметру t.
*/
//---
Vector GeomPolyline::GetDerivativePoint( double t ) const
{
  if ( IsValid() )
	{
		double tcurrent = FixedRange(t);
		double leftParam = 0.0;
    double currentT = std::modf( tcurrent, &leftParam );
    if ( leftParam >= referencedPoints.size() - 1 )
		{
      return  ( referencedPoints[referencedPoints.size() - 1] - referencedPoints[referencedPoints.size() - 2] );
		}
    return ( referencedPoints[leftParam + 1] - referencedPoints[leftParam] );
	}
	else
		return Vector( NAN, NAN );
}


//-----------------------------------------------------------------------------
/**
  Вернуть вторую производную полилиннии по параметру t.
*/
//---
Vector GeomPolyline::Get2DerivativePoint( double ) const
{
  if ( IsValid() )
	{
    return Vector( 0.0, 0.0 );
	} 
	else 
		return Vector( NAN, NAN );
}



//-----------------------------------------------------------------------------
/**
  Сдвинуть по оси x на xShift, по оси y на yShift.
*/
//---
void GeomPolyline::Translate ( double xShift, double yShift )
{
	if ( IsValid() )
	{
    for ( int i = 0; i < referencedPoints.size(); i++ )
      referencedPoints[i].Translate( xShift, yShift );
	}
}


//-----------------------------------------------------------------------------
/**
  Повернуть полинию на угол alphaAng относительно начала координат.
*/
//---
void GeomPolyline::Rotate( double alpha )
{
	if (IsValid())
	{
    for ( int i = 0; i < referencedPoints.size(); i++ )
			referencedPoints[i].Rotate(alpha);
	}
}


//-----------------------------------------------------------------------------
/**
  Масштабировать на xScaling по оси x, на yScaling по оси у.
*/
//---
void GeomPolyline::Scale( double xScaling, double yScaling )
{
  if ( IsValid() )
	{
    for ( int i = 0; i < referencedPoints.size(); i++ )
      referencedPoints[i].Scale( xScaling, yScaling );
	}
}


//-----------------------------------------------------------------------------
/**
  Вернуть расстояние от точки до полилинии.
*/
//---
double GeomPolyline::DistanceToPoint ( Point point ) const
{
	if ( IsValid() )
	{
		double minDistance = std::numeric_limits<double>::max();
    for ( int j = 1; j < referencedPoints.size(); j++ )
		{
      double currentDistance = Distance( referencedPoints[j - 1], referencedPoints[j], point );
      if ( currentDistance < minDistance )
				minDistance = currentDistance;
		}
		return minDistance;
	}
	else
		return -1.;
}


//-----------------------------------------------------------------------------
/**
  Проверить корректность полилинии: нет совпадающих точек, количество точек не равно нулю
*/
//---
bool GeomPolyline::IsValid() const
{
  return referencedPoints.size();
}

//-----------------------------------------------------------------------------
/**
  Вернуть опорные точки, использованные для построения полилинии.
  Соответственно, это точки, на основе которых построена полилиния.
*/
//---
std::vector<Point> GeomPolyline::GetReferensedPoints() const
{
  if ( IsValid() )
	{
		std::vector<Point> refPoints = referencedPoints;
		return refPoints;
	}
}



//-----------------------------------------------------------------------------
/**
Вернуть полилилния для полилинии - это полилиния.
*/
//---
void GeomPolyline::GetAsPolyLine( GeomPolyline &polyLine, double ) const
{
	polyLine.Init( referencedPoints );

}
}
