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
  Curve  (  ),
  center ( Point(0.0, 0.0) ),
  r1     ( 0.0 ),
  r2     ( 0.0),
  axisX  ( 0.0, 0.0 ),
  axisY  ( 0.0, 0.0 )
{
	if ( ther1 > CommonConstantsMath::NULL_TOL && ther2 > CommonConstantsMath::NULL_TOL )
	{
		r1 = ther1;
		r2 = ther2;
		center = thecenter;
        axisX = Vector( center.GetX() + r1, center.GetY() );
        axisX.Rotate( thealpha );
        axisY = Vector( center.GetX(),  center.GetY() + r2 );
        axisY.Rotate( thealpha + CommonConstantsMath::PI/2. );
	}		
}

//-----------------------------------------------------------------------------
//  Конструктор Ellipse по массиву точек.
// ---
Ellipse::Ellipse ( const std::vector<Point>& points ):
  Curve  (),
  center ( Point(0.0,0.0) ),
  r1     ( 0.0 ),
  r2     ( 0.0 ),
  axisX  ( 0.0, 0.0 ),
  axisY  ( 0.0, 0.0 )
{  
	// если точки 2, то это - окружность, создаем окружность
  if ( points.size() == 2 )
  {
    if ( IsCorrectCircleData(points[0], points[1]) )
    {
      center = points[0];
      axisX = points[1] - center;
      const double r = axisX.Lenght();
      r1 = r;
      r2 = r;
    }
  }
  else if ( points.size() >= 3 )
  {
    if ( IsCorrectEllipseData(points[0], points[1], points[2]) )
	  {
      center = points[0];
      // вектор главной оси эллипса
      axisX = points[1] - center;
      double alpha = atan2( axisY.GetY(), axisX.GetX() );
      r1 = axisX.Lenght();

      Point newCoordPoint( std::fabs((points[2] - center).GetX()), std::fabs((points[2] - center).GetY()) );
      newCoordPoint.Rotate( alpha );

      r2 = ( sqrt(fabs((newCoordPoint.GetY()) * (newCoordPoint.GetY()) /
        (1 - (newCoordPoint.GetX()) * (newCoordPoint.GetX()) / (r1 * r1)))) );
      Point coordPointMinorAxis( r2 * cos(180. - alpha ), r2 * sin(180. - alpha) );
      axisY = center - coordPointMinorAxis;
	  }
    else if ( IsCirclePoints(points[0], points[1], points[2]) )
	  {
          center = points[0];
          axisX = points[1] - center;
          axisY = axisX;
          axisY.Rotate( atan2(axisX.GetX(), axisX.GetY()) + CommonConstantsMath::PI );
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
    Vector vector = axisX * r1 * cos(t) + axisY *  r2 * sin(t);

    return center + vector;
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
        Vector vector = axisX * r1 * -sin(t) + axisY *  r2 * cos(t);
            return vector;
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
      Vector vector = axisX * r1 * -cos(t) + axisY *  r2 * -sin(t);
        return vector;
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
      axisX.Rotate( alphaAng );
      axisY.Rotate( alphaAng );
  }
}


//-----------------------------------------------------------------------------
//  Маштабировать эллипс на  xScaling по оси х, yScaling по оси у.
// ---
void Ellipse::Scale( double xScaling, double yScaling )
{
  if ( IsValid() )
  {	
	  center.Scale( xScaling, yScaling );
      axisX.Scale( xScaling, yScaling );
      axisY.Scale( xScaling, yScaling );
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
