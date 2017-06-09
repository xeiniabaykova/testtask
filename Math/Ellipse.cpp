#include "Ellipse.h"
#include <vector>
#include <cmath>
#include "CommonConstantsMath.h"


namespace Math {
namespace {
//-----------------------------------------------------------------------------
/**
  Проверка, образуют ли три точки окружность с цетром в point1.
*/
//---

static bool IsCirclePoints( Point point1, Point point2, Point point3 )
{
   return !( IsEqual(point1, point2) || IsEqual(point2, point3) ) &&
    ( fabs(Distance(point1, point2) - Distance(point1, point3)) < CommonConstantsMath::NULL_TOL ) ;

}

//-----------------------------------------------------------------------------
/**
  Проверка нахождения трех точек на одной линии.
*/
//---

static bool PointsOneLine( Point point1, Point point2, Point point3 )
{
	return (Vector(point1 - point2).IsCollinear(Vector(point2 - point3)));

}


//-----------------------------------------------------------------------------
/**
  Проверка правильности данных для построения эллипса.
*/
//---
static bool CorrectEllipseData( Point point1, Point point2, Point point3 )
{
  return  !( (IsEqual(point1, point2)  ||
              IsEqual(point2, point3)  ||
              IsEqual(point1, point3)) ||
              (PointsOneLine(point1, point2, point3)) );

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
  Конструктор Ellipse по двум радиусам и углу наклона отностительно главной оси.
*/
//---
Ellipse::Ellipse(Point thecenter, double ther1, double ther2, double thealpha) :
	Curve(),
	center(Point(0.0, 0.0)),
	r1(0.0),
	r2(0.0),
	alpha(0.0)
{
	if ( ther1 > CommonConstantsMath::NULL_TOL && ther2 > CommonConstantsMath::NULL_TOL )
	{
		r1 = ther1;
		r2 = ther2;
		center = thecenter;
		alpha = thealpha;
	}		
}

//-----------------------------------------------------------------------------
/**
  Конструктор Ellipse по массиву точек.
*/
//---
Ellipse::Ellipse ( const std::vector<Point>& points ):
  Curve(),
  center( Point(0.0,0.0) ),
  r1    ( 0.0 ),
  r2    ( 0.0 ),
  alpha ( 0.0 )
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
      Point newCoordPoint (std::fabs((points[2] - center).GetX()), std::fabs((points[2] - center).GetY()));
      newCoordPoint.Rotate( alpha );

      double axisB = ( sqrt(fabs((newCoordPoint.GetY()) * (newCoordPoint.GetY()) /
        (1 - (newCoordPoint.GetX()) * (newCoordPoint.GetX()) / (r1 * r1)))) );
		  r2 = axisB;
	  }
    else if ( IsCirclePoints(points[0], points[1], points[2]) )
	  {
      double r = Distance( points[0], points[1] );
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
  if ( IsValid() )
  {
    double tcurrent = FixedParameter( t );

    Point point( r1 * cos(tcurrent), r2 * sin(tcurrent) );
    point.Rotate( alpha );
    return center + point;
  }
  else
    Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
}

//-----------------------------------------------------------------------------
/**
  Возращает границы параметра t для эллипса.
*/
//---


Range Ellipse::GetRange() const
{
  if ( IsValid() )
	{
    return Range( 0.0, 2.0 * CommonConstantsMath::PI );
	}
	else
    Range( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
/**
  Возвращает производную элипса по параметру t.
*/
//---
Vector Ellipse::GetDerivative( double t ) const
{
  if ( IsValid() )
	{
    double tcurrent = FixedParameter( t );
    Vector vector( r1 * -sin(tcurrent), r2 * cos(tcurrent) );
    vector.Rotate( alpha );
		return vector;
	}
	else
    return Vector(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
/**
  Вернуть вторую производную эллипса по параметру t.
*/
//---
Vector Ellipse::Get2Derivative( double t ) const
{
  if ( IsValid() )
	{
    double tcurrent = FixedParameter( t );
    Vector vector( -r1 * cos(tcurrent), -r2 * sin(tcurrent) );
    vector.Rotate( alpha );
		return vector;
	}
	else
    Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
/**
  Сдвинуть эллипс на xShift по оси х, на yShift по оси y.
*/
//---
void Ellipse::Translate( double xShift, double yShift )
{
  if ( IsValid() )
  {
  center.Translate( xShift, yShift );
  }
}


//-----------------------------------------------------------------------------
/**
  Повернуть эллипс на угол alphaAng относительно начала координат.
*/
//---
void Ellipse::Rotate( double alphaAng )
{
  if ( IsValid() )
  {
	  center.Rotate( alphaAng );
      alpha = alpha + alphaAng;
  }
}


//-----------------------------------------------------------------------------
/**
  Маштабировать эллипс на  xScaling по оси х, yScaling по оси у.
*/
//---
void Ellipse::Scale( double xScaling, double yScaling )
{
  if ( IsValid() )
  {	
	  Vector R1( r1 * cos(alpha), r1 * sin(alpha) );
	  Vector R2( r2 * sin(alpha), r2 * cos( alpha) );
	  center.Scale( xScaling, yScaling );
	  R1.Scale( xScaling, yScaling );
	  R2.Scale( xScaling, yScaling );
	  double scaleCoef =  R1.Lenght() / r1;
	  double scaleCoef1 = R2.Lenght() / r2;
	  r1 *= scaleCoef;
	  r2 *= scaleCoef1;
	  double x = R1.GetX();
	  double y = R1.GetY();
	  alpha = atan2(y, x);
  }
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


//-----------------------------------------------------------------------------
/**
  Вернуть опорные точки, использованные для построения эллипса.
  Это точка центра.
*/
//---
std::vector<Point> Ellipse::GetReferensedPoints () const
{
  if ( IsValid() )
  {
    std::vector<Point> refPoints;
    refPoints.push_back( center );
    return refPoints;
  } else
    return std::vector<Point>();
}


//-----------------------------------------------------------------------------
/**
	 Возращает параметр, преобразованный к области определения параметра [0, 2pi], используя свойство переодичности кривой.
*/
//---
double Ellipse::FixedParameter( double t ) const
{
  if ( IsValid() )
  {
    Range range = GetRange();

    while ( t < range.GetStart() )
      t += range.GetEnd() - range.GetStart();
    while ( t > range.GetEnd() )
      t -= range.GetEnd() - range.GetStart();
    return t;
  } else
    return std::numeric_limits<double>::infinity();
}

}
