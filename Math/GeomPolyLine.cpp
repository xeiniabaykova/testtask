#include "GeomPolyline.h"
#include "CommonConstantsMath.h"
#include <cmath>

namespace Math {
namespace {

//-----------------------------------------------------------------------------
/**
  Полилиния считается верной, если количество точек не равно нулю, если точки не совпадают.
*/
//---
static bool CorrectPolylineData( const std::vector<Point>& points )
{
  if ( points.size() == 0 )
    return false;

  for ( int i = 0; i < points.size(); i++ )
    for ( int j = 0; j < points.size(); j++ )
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
//-----------------------------------------------------------------------------
/**
  Конструктор полилинии по опорным точкам. Если точки удовлетворяют условию корректности: их количество не равно нулю и нет совпадающих точек,
  то создается полилиния по воходым точкам. Считаем, что точки в полилинию добаляются в том же порядке, что и находятся в воходном массиве.
*/
//---
GeomPolyline::GeomPolyline( const std::vector<Point>& thePoints ):
  referencedPoints( 0 )
{
  if ( CorrectPolylineData(referencedPoints) )
  {
    referencedPoints = thePoints;
  }
}


//-----------------------------------------------------------------------------
/**
 Вернуть границы параметра t для полинии: [0, количество точек].
*/
//---
Range GeomPolyline::GetRange() const
{
  return Range( 0.0, referencedPoints.size() );
}


//-----------------------------------------------------------------------------
/**
  Вернуть точку по параметру t.
*/
//---
Point GeomPolyline::GetPoint( double t ) const
{
  double currentT = 0.0;
  double leftParam = std::modf( t, &currentT );
  Point startPoint( referencedPoints[leftParam] );
  Vector derection = referencedPoints[leftParam + 1] - referencedPoints[leftParam];
  return startPoint + derection * currentT;
}


//-----------------------------------------------------------------------------
/**
  Вернуть производную полилиннии по параметру t.
*/
//---
Vector GeomPolyline::GetDerivativePoint( double t ) const
{
  double currentT = 0.0;
  double leftParam = std::modf( t, &currentT );
  return ( referencedPoints[leftParam + 1] - referencedPoints[leftParam] );
}


//-----------------------------------------------------------------------------
/**
  Вернуть вторую производную полилиннии по параметру t.
*/
//---
Vector GeomPolyline::Get2DerivativePoint( double ) const
{
  return Vector ( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  Вернуть полилилния для полилинии - это полилиния.
*/
//---
void GeomPolyline::GetAsPolyLine( std::vector<Point> & polyLinePoints, double ) const
{
  polyLinePoints = referencedPoints;
}


//-----------------------------------------------------------------------------
/**
  Сдвинуть по оси x на xShift, по оси y на yShift.
*/
//---
void GeomPolyline::Translate ( double xShift, double yShift )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i].Translate ( xShift, yShift );
}


//-----------------------------------------------------------------------------
/**
  Повернуть полинию на угол alphaAng относительно начала координат.
*/
//---
void GeomPolyline::Rotate( double alpha )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i].Rotate( alpha );
}


//-----------------------------------------------------------------------------
/**
  Масштабировать на xScaling по оси x, на yScaling по оси у.
*/
//---
void GeomPolyline::Scale( double xScaling, double yScaling )
{
  for (int i = 0; i < referencedPoints.size(); i++ )
    referencedPoints[i].Scale ( xScaling,yScaling );
}


//-----------------------------------------------------------------------------
/**
  Вернуть расстояние от точки до полилинии.
*/
//---
double GeomPolyline::DistanceToPoint ( Point point ) const
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
 std::vector<Point> refPoints = referencedPoints;
  return refPoints;
}
}
