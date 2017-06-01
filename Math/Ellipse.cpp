#include "Ellipse.h"
#include <vector>
#include <cmath>
#include "CommonConstantsMath.h"


namespace Math {
namespace {

//double DistanceCircleToPoint( Point center, double r, Point point)
//{
//	if ( Distance(point, center) < r )
//		return r - Distance( point, center );
//	else
//		return Distance( point, center ) - r;
//}
static bool IsCirclePoints( Point point1, Point point2, Point point3 )
{
    return !( IsEqual(point1, point2) || IsEqual(point2, point3) );

  return ( fabs(Distance(point1, point2) - Distance(point1, point3)) < CommonConstantsMath::NULL_TOL ) ;

}

//-----------------------------------------------------------------------------
/**
  Проверка, нахождения трех точек на одной линии.
*/
//---

static bool PointsOneLine( Point point1, Point point2, Point point3 )
{
  return ( fabs((point3.GetX() - point1.GetX()) /
    (point2.GetX() - point1.GetX()) - (point3.GetY() - point1.GetY()) / (point2.GetY() - point1.GetY())) < CommonConstantsMath::NULL_TOL);
}


//-----------------------------------------------------------------------------
/**
  Проверка правильности данных для построения эллипса.
*/
//---
static bool CorrectEllipseData( Point point1, Point point2, Point point3 )
{
  return  !( (IsEqual(point1, point2) ||
    IsEqual(point2, point3) ||
    IsEqual(point1, point3) ) || (PointsOneLine(point1, point2, point3)) );

}


//-----------------------------------------------------------------------------
/**
  Проверка правильности данных для построения окружности.
*/
//---
static bool CorrectCircleData( Point point1, Point point2 )
{
  return !( IsEqual(point1, point2) );
}
}


///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления эллипса.
/**
  Представлет функции для хранения и проведения опраций над эллипсом.
*/
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
/**
  \ru Конструктор Ellipse по двум радиусам и углу наклона отностительно главной оси.
*/
//---
Ellipse::Ellipse( Point center, double r1, double r2, double alpha ):
  Curve(),
  center( center ),
  r1    ( r1     ),
  r2    ( r2     ),
  alpha ( alpha  )
{
  std::vector<Point> points;
  points.push_back( center );
  SetReferensedPoints( points );
}

//-----------------------------------------------------------------------------
/**
  \ru Конструктор Ellipse по массиву точек.
*/
//---
Ellipse::Ellipse ( const std::vector<Point>& points ):
  Curve(),
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
      Point newCoordPoint = (points[2] - center);
      newCoordPoint.Rotate( alpha );

      double axisB = ( sqrt(fabs((newCoordPoint.GetY()) * (newCoordPoint.GetY()) /
        (1 - (newCoordPoint.GetX()) * (newCoordPoint.GetX()) / (r1 * r1)))) );
		  r2 = axisB;
	  }
    else if ( IsCirclePoints(points[0], points[1], points[2]) )
	  {
      double r = Distance( points[0], points[1]);
		  center = points[0];
		  alpha = 0;
		  r1 = r2 = r;
	  }
  
  }
}

//-----------------------------------------------------------------------------
/**
  Возвращается точка по параметру t.
*/
//---
Point Ellipse::GetPoint( double t ) const
{
  Vector vector( r1 * cos(t), r2 * sin(t) );
  vector.Rotate( alpha );
  return center + vector;
}


//-----------------------------------------------------------------------------
/**
  Возращает границы параметра t для эллипса.
*/
//---


Range Ellipse::GetRange() const
{
  return Range( 0.0, 2.0 * CommonConstantsMath::PI );
}


//-----------------------------------------------------------------------------
/**
  Возвращает производную элипса по параметру t.
*/
//-----------------------------------------------------------------------------
Vector Ellipse::GetDerivativePoint( double t ) const
{
  Vector vector( r1 * -sin(t), r2 * cos(t) );
  vector.Rotate( alpha );
  return vector;
}


//-----------------------------------------------------------------------------
/**
  Вернуть вторую производную эллипса по параметру t.
*/
//---
Vector Ellipse::Get2DerivativePoint( double t ) const
{
  Vector vector( -r1 * cos(t), -r2 * sin(t) );
  vector.Rotate( alpha );
  return vector;
}


//-----------------------------------------------------------------------------
/**
  Сдвинуть эллипс на xShift по оси х, на yShift по оси y.
*/
//---
void Ellipse::Translate( double xShift, double yShift )
{
  center.Translate( xShift, yShift );
}


//-----------------------------------------------------------------------------
/**
  Повернуть эллипс на угол alphaAng относительно начала координат.
*/
//---
void Ellipse::Rotate( double alphaAng )
{
  center.Rotate( alphaAng );
}


//-----------------------------------------------------------------------------
/**
  Маштабировать эллипс на  xScaling по оси х, yScaling по оси у.
*/
//---
void Ellipse::Scale( double xScaling, double yScaling )
{
  r1 *= xScaling;
  r2 *= yScaling;
}


//-----------------------------------------------------------------------------
/**
  Проверить корректность эллипса: считаем, что если оба радиуса не равны нулю, то эллипс корректен.
*/
//---
bool Ellipse::IsValid() const
{
  return !( r1 < CommonConstantsMath::NULL_TOL || r2 < CommonConstantsMath::NULL_TOL );
}


//-----------------------------------------------------------------------------
/**
  Вернуть имя, используемое при записи эллипса в файл.
*/
//---
std::string Ellipse::GetName() const
{
  return "Ellipse";
}


//-----------------------------------------------------------------------------
/**
  Вернуть гравный радиус.
*/
//---
double Ellipse::GetMajorRadius() const
{
  return r1;
}


//-----------------------------------------------------------------------------
/**
  Вернуть побочный радиус.
*/
//---
double Ellipse::GetMinorRadius() const
{
  return r2;
}


//-----------------------------------------------------------------------------
/**
  Вернуть угол наклона относительно оси ох.
*/
//---
double Ellipse::GetAlpha() const
{
  return alpha;
}


//-----------------------------------------------------------------------------
/**
  Вернуть центр эллипса.
*/
//---
Point Ellipse::GetCenter() const
{
  return center;
}

}
