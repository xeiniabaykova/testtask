#include "Line.h"
#include "CommonConstantsMath.h"
#include "GeomPolyline.h"
#include <cmath>
#include <algorithm>
#include "Intersector.h"

namespace Math {


//-----------------------------------------------------------------------------
//   Коструктор Line. Записывается начальная точка и конечная точка.
//  Если они совпадают, то создается пустой отрезок,
// ---
Line::Line( Point theStartPoint, Point theEndPoint ):
  Curve(),
  startPoint( Point(0.0, 0.0) ),
  endPoint  ( Point(0.0, 0.0) )
{
  if ( !IsEqual(theStartPoint, theEndPoint) )
	{
      startPoint = theStartPoint;
      endPoint = theEndPoint;
	}
}


//-----------------------------------------------------------------------------
//   Коструктор копии Line. Записывается начальная точка и конечная точка.
// ---
Line::Line(const Line &obj)
{
  startPoint = obj.startPoint;
  endPoint = obj.endPoint;
}


//-----------------------------------------------------------------------------
//   Присвоить один отрезок другому отрезку.
// ---
Line& Line::operator=(const Line &obj)
{
  startPoint = obj.startPoint;
  endPoint = obj.endPoint;
  return *this;
}


//-----------------------------------------------------------------------------
//  Коструктор Line. Записывается начальная точка и конечная точка.
//  Если они совпадают, то создается пустой отрезок, если массив точек имеет размер меньше двух,
//  то записывается пустой отрезок.
// ---
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
//  Возращаются границы параметра t для прямой: [0, 1].
// ---
Range Line::GetRange() const
{
  return Range( 0.0, 1.0 );
}


//-----------------------------------------------------------------------------
//  Вернуть точку по параметру t.
// ---
Point Line::GetPoint( double t ) const
{
  FixParameter( t );
  Vector direction = endPoint - startPoint;
  return startPoint + direction * t;
}


//-----------------------------------------------------------------------------
// Верунуть производную по парметру t.
// ---
Vector Line::GetDerivative( double ) const
{
  return endPoint - startPoint;
}


//-----------------------------------------------------------------------------
// Верунуть вторую производную по парметру t.
// ---
Vector Line::Get2Derivative( double ) const
{
  return Vector( 0.0, 0.0 );
}


//-----------------------------------------------------------------------------
//  Возвращается полилилния для текущего отрезка - это точки начала и конца.
// ---
void Line::GetAsPolyLine(GeomPolyline &polyLine, double) const
{
  std::vector<Point> points;
  points.push_back( startPoint );
  points.push_back( endPoint );
  polyLine.Init( points );

}

//-----------------------------------------------------------------------------
//  Сдвинуть по осям x, y на xShift, yShift.
// ---
void Line::Translate( double xShift, double yShift )
{
  startPoint.Translate( xShift, yShift );
  endPoint.Translate( xShift, yShift );
}


//-----------------------------------------------------------------------------
//  Повернуть на угол alpha относительно начала координат.
// ---
void Line::Rotate( double alpha )
{
  startPoint.Rotate( alpha );

  endPoint.Rotate( alpha );
}


//-----------------------------------------------------------------------------
//  Маштабировать по оси х на xScaling, по оси y на yScaling.
// ---
void Line::Scale( double xScaling, double yScaling )
{
  startPoint.Scale( xScaling, yScaling );
  endPoint.Scale( xScaling, yScaling );
}


//-----------------------------------------------------------------------------
//  Вернуть имя, используемое при записи отрезка в файл.
// ---
std::string Line::GetName() const
{
  return "Line";
}


//-----------------------------------------------------------------------------
//  Проверка на корректность отрезка (начальная и конечная точка не совпадают).
// ---
bool Line::IsValid() const
{
  return ( !IsEqual(startPoint, endPoint) );
}


//-----------------------------------------------------------------------------
//  Вернуть опорные точки, использованные для построения отрезка.
//  Соответственно, это точки начала и колнца отрезка.
// ---
std::vector<Point> Line::GetReferensedPoints() const
{
  std::vector<Point> refPoints;
  refPoints.push_back( startPoint );
  refPoints.push_back( endPoint );
  return refPoints;
}


//-----------------------------------------------------------------------------
//  Вернуть начальную точку отрезка.
// ---
Point Line::GetStartPoint() const
{
  return startPoint;
}

//-----------------------------------------------------------------------------
//  Вернуть конечную точку отрезка.
// ---
Point Line::GetEndPoint() const
{
  return endPoint;
}


//-----------------------------------------------------------------------------
//  Вернуть замкунтость отрезка.
// ---
bool Line::IsClosed() const
{
  return false;
}


//-----------------------------------------------------------------------------
//  Вернуть расстояние от точки до прямой.
// ---
double Line::DistanceToPoint( Point point ) const
{
   const Vector v = endPoint - startPoint;
   const Vector w = point - startPoint;

   const double c1 = v * w;
   if ( c1 <= 0 )
       return Distance( point, startPoint );

   const double c2 = v * v;
   if ( c2 <= c1 )
       return Distance( point, endPoint );

   const double b = c1 / c2;
   const Point Pb = startPoint + v * b;
   return Distance( point, Pb );

}


Curve::CurveType Line::GetType() const
{
  return Curve::LineType;
}

bool Line::operator==(const Line& obj) const
{
  return IsEqual(startPoint, obj.GetStartPoint()) && IsEqual(endPoint, obj.GetEndPoint());
}

//-----------------------------------------------------------------------------
//  Вернуть расстояние между отрезками.
// ---
double Distance( const Line& line1, const Line& line2, Point& closestPoint )
{
  if (line1.DistanceToPoint(line2.GetStartPoint()) > line1.DistanceToPoint(line2.GetEndPoint()))
    closestPoint = line2.GetStartPoint();
  else
    closestPoint = line2.GetEndPoint();
  return( std::min(line1.DistanceToPoint(line2.GetStartPoint()) , line1.DistanceToPoint(line2.GetEndPoint())) );

}

}
