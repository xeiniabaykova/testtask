#include "GeomPolyline.h"
#include "Line.h"
#include <cmath>
#include <limits>

namespace Math {
namespace {

//-----------------------------------------------------------------------------
//  Полилиния считается верной, если количество точек не равно нулю, если точки не совпадают ( кроме траничных - полилиния может быть замкнутой).
// ---
static bool IsCorrectPolylineData( const std::vector<Point>& points )
{
  if ( points.size() < 2 )
    return false;
  if ( points.size() == 2 && IsEqual(points[0], points[1]) )
	  return false;
  for ( size_t i = 0; i < points.size() - 2; i++ )
    if ( IsEqual(points[i], points[i+2]) || IsEqual(points[i], points[i+1]) )
      return false;
  // две последние точки обрабатываем отдельно, проверяем что они не совпадают
  if ( IsEqual(points[points.size() - 1], points[points.size() - 2]) )
    return false;
  return true;
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
  Curve(),
  referencedPoints()
{
}

//-----------------------------------------------------------------------------
//  Конструктор полилинии по опорным точкам. Если точки удовлетворяют условию корректности: их количество не равно нулю и нет совпадающих точек,
//  то создается полилиния по воходым точкам. Считаем, что точки в полилинию добаляются в том же порядке, что и находятся в входном массиве.
// ---
GeomPolyline::GeomPolyline( const std::vector<Point>& thePoints ):
   Curve(),
  referencedPoints()
{
 Init( thePoints );
}

//-----------------------------------------------------------------------------
//  Инициализация полилинии по опорным точкам. Если точки удовлетворяют условию корректности: их количество не равно нулю и нет совпадающих точек,
//  то создается полилиния по воходым точкам. Считаем, что точки в полилинию добаляются в том же порядке, что и находятся в входном массиве.
// ---
void GeomPolyline::Init( const std::vector<Point>& theReferencedPoints )
{
	referencedPoints.clear();
  if ( IsCorrectPolylineData(theReferencedPoints) )
  {	 
    referencedPoints = theReferencedPoints;
  }
}

//-----------------------------------------------------------------------------
// Вернуть границы параметра t для полинии: [0, количество точек].
// ---
Range GeomPolyline::GetRange() const
{
	if (IsValid())
	{
    return Range( 0.0, referencedPoints.size() - 1 );
	}
	else
		return Range( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
// Вернуть точку по параметру t.
// ---
Point GeomPolyline::GetPoint( double t ) const
{
	if ( IsValid() ) 
	{
    FixParameter(t);
    double leftParam = 0.0;
    t = std::modf( t, &leftParam );
    leftParam = static_cast<size_t>( leftParam );
    if ( leftParam >= referencedPoints.size() - 1 )
		{
      return  Point( referencedPoints[referencedPoints.size() - 1] );
		}
		Point startPoint( referencedPoints[leftParam] );
		Vector derection = referencedPoints[leftParam + 1] - referencedPoints[leftParam];
    return startPoint + derection * t;
	}
	else
		return Point( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
// Вернуть производную полилиннии по параметру t.
// ---
Vector GeomPolyline::GetDerivative( double t ) const
{
  if ( IsValid() )
	{
    FixParameter( t );
		double leftParam = 0.0;
    std::modf( t, &leftParam );
    leftParam = static_cast<size_t>( leftParam );
    if ( leftParam >= referencedPoints.size() - 1 )
		{
      return  ( referencedPoints[referencedPoints.size() - 1] - referencedPoints[referencedPoints.size() - 2] );
		}
    return ( referencedPoints[leftParam + 1] - referencedPoints[leftParam] );
	}
	else
		return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
// Вернуть вторую производную полилиннии по параметру t.
// ---
Vector GeomPolyline::Get2Derivative( double ) const
{
  if ( IsValid() )
	{
    return Vector( 0.0, 0.0 );
	} 
	else 
		return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}



//-----------------------------------------------------------------------------
// Сдвинуть по оси x на xShift, по оси y на yShift.
// ---
void GeomPolyline::Translate ( double xShift, double yShift )
{
	if ( IsValid() )
	{
    for ( size_t i = 0; i < referencedPoints.size(); i++ )
      referencedPoints[i].Translate( xShift, yShift );
	}
  if ( !IsValid() )
    referencedPoints.clear();
}


//-----------------------------------------------------------------------------
// Повернуть полинию на угол alphaAng относительно начала координат.
// ---
void GeomPolyline::Rotate( double alpha )
{
	if (IsValid())
	{
    for ( size_t i = 0; i < referencedPoints.size(); i++ )
      referencedPoints[i].Rotate( alpha );
	}
  if ( !IsValid() )
    referencedPoints.clear();
}


//-----------------------------------------------------------------------------
// Масштабировать на xScaling по оси x, на yScaling по оси у.
// ---
void GeomPolyline::Scale( double xScaling, double yScaling )
{
  if ( IsValid() )
	{
    for ( size_t i = 0; i < referencedPoints.size(); i++ )
      referencedPoints[i].Scale( xScaling, yScaling );
	}
  if ( !IsValid() )
    referencedPoints.clear();
}


//-----------------------------------------------------------------------------
// Вернуть расстояние от точки до полилинии.
// ---
double GeomPolyline::DistanceToPoint ( Point point ) const
{
  double minDistance = std::numeric_limits<double>::max();
  double currentDistance = std::numeric_limits<double>::max();
	if ( IsValid() )
	{
    for ( size_t j = 1; j < referencedPoints.size() && currentDistance > CommonConstantsMath::NULL_TOL; j++ )
		{
      currentDistance = Line( referencedPoints[j - 1], referencedPoints[j]).DistanceToPoint( point );
      if ( currentDistance < minDistance )
				minDistance = currentDistance;
		}		
	}
  return minDistance;
}


//-----------------------------------------------------------------------------
// Проверить корректность полилинии: нет совпадающих точек, количество точек не равно нулю
// ---
bool GeomPolyline::IsValid() const
{
  return !referencedPoints.empty();
}

//-----------------------------------------------------------------------------
//  Вернуть опорные точки, использованные для построения полилинии.
//  Соответственно, это точки, на основе которых построена полилиния.
// ---
std::vector<Point> GeomPolyline::GetReferensedPoints() const
{
  std::vector<Point> refPoints;
  if ( IsValid() )
	{
    refPoints = referencedPoints;
	}
  return refPoints;
}



//-----------------------------------------------------------------------------
// Вернуть полилилния для полилинии - это полилиния.
// ---
void GeomPolyline::GetAsPolyLine( GeomPolyline &polyLine, double ) const
{
	polyLine.Init( referencedPoints );

}



//-----------------------------------------------------------------------------
//  Вернуть имя, используемое при записи полилинии в файл.
// ---
std::string GeomPolyline::GetName() const
{
  return "Polyline";
}


//-----------------------------------------------------------------------------
//  Вернуть имя, используемое при записи полилинии в файл.
// ---
bool GeomPolyline::IsClosed() const
{
  return ( Distance(referencedPoints[referencedPoints.size() - 1], referencedPoints[0])
      < CommonConstantsMath::NULL_TOL );
}
}
