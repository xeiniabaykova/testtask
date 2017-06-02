#include "Line.h"
#include "CommonConstantsMath.h"
#include <cmath>

namespace Math {


//-----------------------------------------------------------------------------
/**
   Коструктор Line. Записывается начальная точка и конечная точка.
  Если они совпадают, то создается пустой отрезок,
*/
//---
Line::Line( Point theStartPoint, Point theEndPoint ):
  Curve(),
  startPoint( Point(0.0, 0.0) ),
  endPoint  ( Point(0.0, 0.0) )
{
  if ( !IsEqual(startPoint, endPoint) )
	{
     startPoint = theStartPoint;
     endPoint = theEndPoint;
	}
}


//-----------------------------------------------------------------------------
/**
  Коструктор Line. Записывается начальная точка и конечная точка.
  Если они совпадают, то создается пустой отрезок, если массив точек имеет размер меньше двух,
  то записывается пустой отрезок.
*/
//---
Line::Line( const std::vector<Point>& points ):
  Curve(),
  startPoint( Point(0.0, 0.0) ),
  endPoint  ( Point(0.0, 0.0) )
{
  if ( points.size() >= 2 && !IsEqual(points[0], points[1]) )
  {
    startPoint = points[0];
    endPoint = points[1];
  }
}

//-----------------------------------------------------------------------------
/**
  Возращаются границы параметра t для прямой: [0, 1].
*/
//---
Range Line::GetRange() const
{
  return Range( 0.0, 1.0 );
}


//-----------------------------------------------------------------------------
/**
  Вернуть точку по параметру t.
*/
//---
Point Line::GetPoint( double t ) const
{
  Vector direction = endPoint - startPoint;
  return startPoint + direction * t;
}


//-----------------------------------------------------------------------------
/**
  Верунуть производную по парметру t.
*/
//---
Vector Line::GetDerivativePoint( double ) const
{
  return endPoint - startPoint;
}


//-----------------------------------------------------------------------------
/**
  Верунуть вторую производную по парметру t.
*/
//---
Vector Line::Get2DerivativePoint( double ) const
{
  return Vector( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
/**
  Возвращается полилилния для текущего отрезка - это точки начала и конца.
*/
//---
void Line::GetAsPolyLine( std::vector<Point> & polyLinePoints, double ) const
{
  polyLinePoints.clear();
  polyLinePoints.push_back( startPoint );
  polyLinePoints.push_back( endPoint );
}


//-----------------------------------------------------------------------------
/**
  Сдвинуть по осям x, y на xShift, yShift.
*/
//---
void Line::Translate( double xShift, double yShift )
{
  startPoint.Translate( xShift, yShift );
  endPoint.Translate( xShift, yShift );
}


//-----------------------------------------------------------------------------
/**
  Повернуть на угол alpha относительно начала координат.
*/
//---
void Line::Rotate( double alpha )
{
  startPoint.Rotate( alpha );

  endPoint.Rotate( alpha );
}


//-----------------------------------------------------------------------------
/**
  Маштабировать по оси х на xScaling, по оси y на yScaling.
*/
//---
void Line::Scale( double xScaling, double yScaling )
{
  startPoint.Scale( xScaling, yScaling );
  endPoint.Scale( xScaling, yScaling );
}


//-----------------------------------------------------------------------------
/**
  Вернуть имя, используемое при записи отрезка в файл.
*/
//---
std::string Line::GetName() const
{
  return "Line";
}


//-----------------------------------------------------------------------------
/**
  Проверка на корректность отрезка (начальная и конечная точка не совпадают).
*/
//---
bool Line::IsValid() const
{
  return ( !IsEqual(startPoint, endPoint) );
}


//-----------------------------------------------------------------------------
/**
  Вернуть опорные точки, использованные для построения отрезка.
  Соответственно, это точки начала и колнца отрезка.
*/
//---
std::vector<Point> Line::GetReferensedPoints () const
{
  std::vector<Point> refPoints;
  refPoints.push_back( startPoint );
  refPoints.push_back( endPoint );
  return refPoints;
}
}
