#include "Ellipse.h"
#include <vector>
#include <cmath>
#include <limits>
#include "CommonConstantsMath.h"


namespace Math {
namespace {


//-----------------------------------------------------------------------------
// Проверка, образуют ли три точки окружность с цетром в point1.
// ---
static bool IsCirclePoints( Point point1, Point point2, Point point3 )
{
   return !( IsEqual(point1, point2)) &&
    ( fabs(Distance(point1, point2) - Distance(point1, point3)) < CommonConstantsMath::NULL_TOL ) ;

}


//-----------------------------------------------------------------------------
//  Проверка нахождения трех точек на одной линии.
// ---
static bool IsPointsOneLine( Point point1, Point point2, Point point3 )
{
  return ( Vector(point1 - point2).IsCollinear(Vector(point2 - point3)) );
}


//-----------------------------------------------------------------------------
// Проверка правильности данных для построения эллипса.
// ---
static bool IsCorrectEllipseData( Point point1, Point point2, Point point3 )
{
  return  !( (IsEqual(point1, point2)  ||
              IsEqual(point2, point3)  ||
              IsEqual(point1, point3)) ||
              (IsPointsOneLine(point1, point2, point3)) );

}


//-----------------------------------------------------------------------------
// Проверка правильности данных для построения окружности.
// ---
static bool IsCorrectCircleData( Point point1, Point point2 )
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
// Конструктор Ellipse по двум радиусам и углу наклона отностительно главной оси.
// ---
Ellipse::Ellipse( Point thecenter, double ther1, double ther2, double thealpha ) :
  Curve          (  ),
  center         ( Point(0.0, 0.0) ),
  r1             ( 0.0 ),
  r2             ( 0.0),
  alphaMajorAxis ( 0.0),
  alphaMinorAxis ( 0.0)
{
  if ( ther1 > CommonConstantsMath::NULL_TOL && ther2 > CommonConstantsMath::NULL_TOL )
    {
      r1 = ther1;
      r2 = ther2;
      center = thecenter;
      alphaMajorAxis = thealpha;
      alphaMinorAxis = alphaMajorAxis + CommonConstantsMath::PI / 2.;
    }
}


//-----------------------------------------------------------------------------
// Конструктор Ellipse по двум радиусам и углу наклона отностительно главной оси.
// ---
Ellipse::Ellipse ( Point thecenter, double ther1, double ther2, double alphaMajor, double alphaMinor ):
  Curve          (  ),
  center         ( Point(0.0, 0.0) ),
  r1             ( 0.0 ),
  r2             ( 0.0 ),
  alphaMajorAxis ( 0.0 ),
  alphaMinorAxis ( 0.0 )
{
  if ( ther1 > CommonConstantsMath::NULL_TOL && ther2 > CommonConstantsMath::NULL_TOL )
  {
    r1 = ther1;
    r2 = ther2;
    center = thecenter;
    alphaMajorAxis = alphaMajor;
    alphaMinorAxis = alphaMinor;
  }
}


//-----------------------------------------------------------------------------
//  Конструктор Ellipse по массиву точек.
// ---
Ellipse::Ellipse ( const std::vector<Point>& points ):
  Curve          (),
  center         ( Point(0.0,0.0) ),
  r1             ( 0.0 ),
  r2             ( 0.0 ),
  alphaMajorAxis ( 0.0 ),
  alphaMinorAxis ( 0.0)
{  
	// если точки 2, то это - окружность, создаем окружность
  if ( points.size() == 2 )
  {
    if ( IsCorrectCircleData(points[0], points[1]) )
    {
      center = points[0];
      const Vector v = points[1] - center;
      const double r = v.Lenght();
      r1 = r;
      r2 = r;
	  alphaMajorAxis = 0.;
	  alphaMinorAxis = CommonConstantsMath::PI / 2.;
    }
  }
  else if ( points.size() >= 3 )
  {
    if ( IsCorrectEllipseData(points[0], points[1], points[2]) )
	  {
        center = points[0];
        // вектор главной оси эллипса
        const Vector v  = points[1] - center;
        // угол наклона между главной осью эллипса и осью ох
        alphaMajorAxis = atan2( v.GetY(), v.GetX() );
        alphaMinorAxis = alphaMajorAxis + CommonConstantsMath::PI / 2.;
        r1 = v.Lenght();

      Point newCoordPoint( (points[2] - center).GetX(), (points[2] - center).GetY() );
      newCoordPoint.Rotate( -alphaMajorAxis );

        r2 = ( sqrt(fabs((newCoordPoint.GetY()) * (newCoordPoint.GetY()) /
            (1 - (newCoordPoint.GetX()) * (newCoordPoint.GetX()) / (r1 * r1)))) );
	  }
    else if ( IsCirclePoints(points[0], points[1], points[2]) )
	  {
         center = points[0];
         alphaMajorAxis = 0.;
         alphaMinorAxis = alphaMajorAxis + CommonConstantsMath::PI / 2.;
         r1 = r2 = Distance( points[0], points[1] );
	  }
  
  }
}


//-----------------------------------------------------------------------------
//  Возвращается точка по параметру t.
// ---
Point Ellipse::GetPoint( double t ) const
{
  if ( IsValid() )
  {
    FixParameter( t );
    Point point( r1 * cos(t) , r2 * sin(t) );
    double x = point.GetX() * cos( alphaMajorAxis ) + point.GetY() * cos( alphaMinorAxis );
    double y = point.GetX() * sin( alphaMajorAxis ) + point.GetY() * sin( alphaMinorAxis );
    return center + Point( x, y );
  }
  else
    Point( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
// Возращает границы параметра t для эллипса.
// ---
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
// Возвращает производную элипса по параметру t.
// ---
Vector Ellipse::GetDerivative( double t ) const
{
  if ( IsValid() )
	{
      FixParameter( t );
      Vector vector( r1 * -sin(t), r2 * cos(t) );
      double x = vector.GetX() * cos( alphaMajorAxis ) + vector.GetY() * cos( alphaMinorAxis );
      double y = vector.GetX() * sin( alphaMajorAxis ) + vector.GetY() * sin( alphaMinorAxis );
      return Vector( x, y );
     }
  else
    return Vector(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
//  Вернуть вторую производную эллипса по параметру t.
// ---
Vector Ellipse::Get2Derivative( double t ) const
{
  if ( IsValid() )
	{
      FixParameter( t );
      Vector vector( -r1 * cos(t), -r2 * sin(t) );
      double x = vector.GetX() * cos( alphaMajorAxis ) + vector.GetY() * cos( alphaMinorAxis );
      double y = vector.GetX() * sin( alphaMajorAxis ) + vector.GetY() * sin( alphaMinorAxis );
      return Vector( x, y );
	}
  else
    Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
//  Сдвинуть эллипс на xShift по оси х, на yShift по оси y.
// ---
void Ellipse::Translate( double xShift, double yShift )
{
  if ( IsValid() )
  {
    center.Translate( xShift, yShift );
  }
}


//-----------------------------------------------------------------------------
// Повернуть эллипс на угол alphaAng относительно начала координат.
// ---
void Ellipse::Rotate( double alphaAng )
{
  if ( IsValid() )
  {
	  center.Rotate( alphaAng );
      alphaMajorAxis += alphaAng;
	  alphaMinorAxis += alphaAng;
  }
}


//-----------------------------------------------------------------------------
//  Маштабировать эллипс на  xScaling по оси х, yScaling по оси у.
// ---
void Ellipse::Scale( double xScaling, double yScaling )
{
  if ( IsValid() )
  {	
    Vector R1( cos(alphaMajorAxis), cos(alphaMinorAxis) );
    Vector R2( sin(alphaMajorAxis), sin(alphaMinorAxis ) );
	center.Scale( xScaling, yScaling );
	R1.Scale( xScaling, yScaling );
    R2.Scale( xScaling, yScaling );
    r1 *= R1.Lenght();
    r2 *= R2.Lenght();
	alphaMajorAxis = atan2( R1.GetY(), R1.GetX() );
	alphaMinorAxis = atan2( R2.GetY(), R2.GetX() );
  }
}


//-----------------------------------------------------------------------------
// Проверить корректность эллипса: считаем, что если оба радиуса не равны нулю, то эллипс корректен.
// ---
bool Ellipse::IsValid() const
{
  return ( r1 > CommonConstantsMath::NULL_TOL && r2 > CommonConstantsMath::NULL_TOL );
}


//-----------------------------------------------------------------------------
//  Вернуть имя, используемое при записи эллипса в файл.
// ---
std::string Ellipse::GetName() const
{
  return "Ellipse";
}


//-----------------------------------------------------------------------------
// Вернуть гравный радиус.
// ---
double Ellipse::GetMajorRadius() const
{
  return r1;
}


//-----------------------------------------------------------------------------
//  Вернуть побочный радиус.
// ---
double Ellipse::GetMinorRadius() const
{
  return r2;
}


//-----------------------------------------------------------------------------
//  Вернуть угол наклона главной оси относительно оси ох.
// ---
double Ellipse::GetAlphaMajorAxis() const
{
	return alphaMajorAxis;
}


//-----------------------------------------------------------------------------
//  Вернуть угол наклона побочной оси относительно оси ох.
// ---
double Ellipse::GetAlphaMinorAxis() const
{
	return alphaMinorAxis;

}
//-----------------------------------------------------------------------------
//  Вернуть центр эллипса.
// ---
Point Ellipse::GetCenter() const
{
  return center;
}


//-----------------------------------------------------------------------------
//  Вернуть опорные точки, использованные для построения эллипса.
//  Это точка центра.
// ---
std::vector<Point> Ellipse::GetReferensedPoints() const
{
  if ( IsValid() )
  {
    std::vector<Point> refPoints;
    refPoints.push_back( center );
    return refPoints;
  }
  else
    return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Вернуть опорные точки, использованные для построения эллипса.
//  Это точка центра.
// ---
bool Ellipse::IsClosed() const
{
	return true;
}
}
