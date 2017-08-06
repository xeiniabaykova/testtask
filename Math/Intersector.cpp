#include "Intersector.h"
#include "Line.h"
#include "Ellipse.h"
#include "GeomPolyline.h"
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <unordered_set>
#include <functional>
#include <unordered_map>
#include <array>
#include <math.h>
namespace Math {

///////////////////////////////////////////////////////////////////////////////
//
/// Метод Ньютона и вспомогательные функции.
///////////////////////////////////////////////////////////////////////////////

using Matrix22 = std::array<std::array<double, 2>, 2>;
//-----------------------------------------------------------------------------
//  Найти значение матрицы Гессе для квадрата расстояния между кривыми в точке (t1, t2).
// ---
static Matrix22 CountingHessian( const Curve& curve1, const Curve& curve2, double t1, double t2 )
{
  const Point t1Point = curve1.GetPoint( t1 );
  const double x1 = t1Point.GetX();
  const double y1 = t1Point.GetY();

  const Point t1PointD = curve1.GetDerivative( t1 );
  const double x1d = t1PointD.GetX();
  const double y1d = t1PointD.GetY();

  const Point t1Point2D = curve1.Get2Derivative( t1 );
  const double x1dd = t1Point2D.GetX();
  const double y1dd = t1Point2D.GetY();

  const Point t2Point = curve2.GetPoint( t2 );
  const double x2 = t2Point.GetX();
  const double y2 = t2Point.GetY();

  const Point t2PointD = curve2.GetDerivative( t2 );
  const double x2d = t2PointD.GetX();
  const double y2d = t2PointD.GetY();

  const Point t2Point2D = curve2.Get2Derivative( t2 );
  const double x2dd = curve2.Get2Derivative( t2 ).GetX();
  const double y2dd = curve2.Get2Derivative( t2 ).GetY();

  Matrix22 hessian;
  hessian[0][0] = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  hessian[0][1] = -2. * x2d * x1d - 2. * y2d * y1d;
  hessian[1][0] = -2. *x1d * x2d - 2. * y1d * y2d;
  hessian[1][1] = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;

  return std::move( hessian );
}


//-----------------------------------------------------------------------------
//  Вернуть обратную матрицу. Предполагается, что матрица невырождена.
// ---
static Matrix22& InverseMatrix( Matrix22& matrix )
{
  Matrix22 result( matrix );
  const double determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  matrix[0][0] = result[1][1] / determinant;
  matrix[0][1] = -result[1][0] / determinant;
  matrix[1][0] = -result[0][1] / determinant;
  matrix[1][1] = result[0][0] / determinant;
  return matrix;
}


//-----------------------------------------------------------------------------
//  Вернуть градиент для квадрата расстояния между кривыми в заданной точке.
// ---
static Vector Gradient( const Curve& curve1, const Curve& curve2, std::pair<double, double> paramsCurve )
{
  const Point point1 = curve1.GetPoint( paramsCurve.first );
  const Vector grad1 = curve1.GetDerivative( paramsCurve.first );

  const Point point2 = curve2.GetPoint( paramsCurve.second );
  const Vector grad2 = curve2.GetDerivative( paramsCurve.second );
  const double aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  const  double aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
  return Vector( aResultT1, aResultT2 );
}


//-----------------------------------------------------------------------------
//  Вернуть расстояние между двумя параметрическими точками.
// ---
static double Distance( std::pair<double, double> p1, std::pair<double, double> p2 )
{
  return sqrt( ( p1.first - p2.first ) * ( p1.first - p2.first ) + ( p1.second - p2.second ) * ( p1.second - p2.second ) );
}


//-----------------------------------------------------------------------------
// Сложить две параметрические точки.
// ---
static std::pair<double, double> operator + ( std::pair<double, double>& point1, std::pair<double, double>& point2 )
{
  return std::make_pair( point1.first + point2.first, point1.second + point2.second );
}


//-----------------------------------------------------------------------------
//  Запустить метод Ньютона для поиска локального минимума квадарта расстояния между кривыми с начальной точкой в startValue.
// ---
static std::pair<double, double> NewtonMethod( const Curve& curve1, const Curve& curve2, std::pair<double, double> startValue )
{
  std::pair<double, double> currentPoint = startValue;
  std::pair<double, double> newPoint;
  Matrix22 lastNonZeroMatrix = { 1.,1.,1.,1. };
  for ( size_t i = 0; i < CommonConstantsMath::NUMBER_NEWTON_METHOD; i++ )
  {
    auto invHessian = CountingHessian( curve1, curve2, currentPoint.first, currentPoint.second );
    if ( fabs( invHessian[0][0] * invHessian[1][1] - invHessian[0][1] * invHessian[1][0] ) > CommonConstantsMath::NULL_TOL )
      lastNonZeroMatrix = invHessian;
    else
      invHessian = lastNonZeroMatrix;
    invHessian = InverseMatrix( invHessian );
    const Vector grad = Gradient( curve1, curve2, currentPoint );
    std::pair<double, double> step( invHessian[0][0] * -grad.GetX() + invHessian[0][1] * -grad.GetY(), invHessian[1][0] *
                                    -grad.GetX() + invHessian[1][1] * -grad.GetY() );
    newPoint = currentPoint + step;
    if ( fabs( Distance(newPoint, currentPoint) ) < CommonConstantsMath::ACCURANCY_METHOD_NEWTON )
      break;
    currentPoint = newPoint;
  }
  return newPoint;
}


//-----------------------------------------------------------------------------
// Вернуть первую кривую.
// ---
const Curve& CurveIntersectionData::GetCurve1() const
{
  return curve1;
}


//-----------------------------------------------------------------------------
// Вернуть вторую кривую.
// ---
const Curve& CurveIntersectionData::GetCurve2() const
{
  return curve2;
}


//-----------------------------------------------------------------------------
// Вернуть набор параметров, при которых кривые пересекаются.
// ---
std::pair<double, double> CurveIntersectionData::GetParams() const
{
  return std::make_pair( paramCurve1, paramCurve2 );
}

///////////////////////////////////////////////////////////////////////////////
//
/// Алгоритм быстрого пересечения отрезков на основе заметающей линии. Описан в Preparata,Sheimos, 1989 г, стр 326.
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Проверить, содержит ли отрезок точку с заданной координатой х.
// ---
static bool IsXinSegment( const Line& line, double x )
{
  return ( x - line.GetStartPoint().GetX() ) * ( line.GetEndPoint().GetX() - x ) >= 0;
}


//-----------------------------------------------------------------------------
// Проверить, содержит ли отрезок точку с заданной координатой y.
// ---
static bool IsYinSegment( const Line& line, double y )
{
  return ( y - line.GetStartPoint().GetY() ) * ( line.GetEndPoint().GetY() - y ) >= 0;
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух отрезков.
// ---
static bool IntersectLines( const Math::Line& lineCurveFirst, const Math::Line& lineCurveSecond, Point& intersectionPoint  )
{

  const Vector dir1 = lineCurveFirst.GetDerivative( 0. );
  const Vector dir2 = lineCurveSecond.GetDerivative( 0. );
  const Point start1 = lineCurveFirst.GetStartPoint();
  const Point start2 = lineCurveSecond.GetStartPoint();
  const Point end1 = lineCurveFirst.GetEndPoint();
  const Point end2 = lineCurveSecond.GetEndPoint();
  double rX = start1.GetX() - start2.GetX();
  double rY = start1.GetY() - start2.GetY();



  if ( !dir1.IsCollinear( dir2 ) )
  {
    double det = ( dir2.GetX() *-dir1.GetY() + dir1.GetX() * dir2.GetY() );
    double det1 = rY * dir2.GetX() - dir2.GetY() * rX;
    double det2 = -dir1.GetY() * rX + dir1.GetX() *  rY;
    double  t1Intersect = det1 / det;
    double  t2Intersect = det2 / det;

    if ( 0. <= t1Intersect && 1. >= t1Intersect && 0. <= t2Intersect && 1. >= t2Intersect )
    {
      intersectionPoint = start1 + dir1 * t1Intersect;
      return true;
    }
    else
      return false;
  }
  else
    return false;
}


//-----------------------------------------------------------------------------
//  Перечисление для определения типа события точки:
// Точка может быть левым концом отрезка, правым концом отрезка, точкой переcечения.
// ---
enum TypeEvent
{
  Intersection, // Точка пересечения отрезков.
  Left,         // Левый конец отрезка.
  Right        // Правый конец отрезка.
};


//-----------------------------------------------------------------------------
//  Отрезок и данные, необходимые для нахождения пересечения отрезков. Так же хранится указатель на полилинию:
//  это необходимо, чтобы не учитывать перечечения отрезков внутри полилинии.
// ---
struct LineData
{
  Line                      line;      // Отрезок.
  const GeomPolyline&       polyline;  // Полилиния, которой принадлежит отрезок.
  size_t                    numParam;  // Номер полилинии в списке полилиний.
  Curve                     &curve;    // Кривая, соответствующая отрезку.
  double                    leftParam; // Параметр исходной кривой, соответсующий отрезку полилинии.
  double                    key;       // Ключ для сравнения отрезков - по умолчанию координата y левой точки.
  LineData    ( Line theLine, const GeomPolyline& thePolyline, double theLeftParam, size_t thenumParam, double theKey, Curve& theCurve ):
    line      ( theLine ),
    polyline  ( thePolyline ),
    leftParam ( theLeftParam ),
    numParam  ( thenumParam ),
    key       ( theKey ),
    curve     ( theCurve )
  {}
  LineData() = default;
};


//-----------------------------------------------------------------------------
//  Вспомогательная структура для алгоритма быстрого пересечения отрезков.
// Хранится точка текущего события, тип события.
// в случае, когда тип события пересечение - хранятся отрезки.
// ---
struct PointEvent
{
  TypeEvent type;   // Тип точки события.
  Point     point;   // Точка события.
  LineData  *s1;     // Отрезок, соответствующий точке события.
  LineData  *s2;     // Если точка события является точкой пересечения, то храним второй отрезок тоже.
  PointEvent( Point thePoint, LineData *theS1, TypeEvent theType ):
    point( thePoint ),
    type( theType ),
    s1( theS1 )
  {}
};


//-----------------------------------------------------------------------------
// Проверить точки на равенство.
// ---
static bool IsSame( const Point& lhs, const Point& rhs )
{

  return ( lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY() );
}


//-----------------------------------------------------------------------------
// Сравнить точки лексикографически.
// ---
static bool IsLexLessFromX( const Point& lhs, const Point& rhs )
{
  if ( IsSame( lhs, rhs ) )
    return ( lhs.GetY() < rhs.GetY() );
  else
    return ( lhs.GetX() < rhs.GetX() );
}


//-----------------------------------------------------------------------------
// Сравнить точки лексикографически по y.
// ---
static bool IsLexLessFromY( const Point& lhs, const Point& rhs )
{
  if ( IsSame( lhs, rhs ) )
    return false;
  else
    return ( lhs.GetY() < rhs.GetY() );
}
//-----------------------------------------------------------------------------
//  Функтор лексиграфического упорядочивания точек по возрастанию.
// ---
struct IsLexLessX
{
  inline bool operator()( const PointEvent &lhs, const PointEvent& rhs ) const
  {
    if ( IsSame( lhs.point, rhs.point ) )
    {
      return IsLexLessFromX( lhs.s1->line.GetEndPoint(), rhs.s1->line.GetEndPoint() );
    }
    return IsLexLessFromX( lhs.point, rhs.point );
  }
};


//-----------------------------------------------------------------------------
// Получить по координате х, принадлежащей отрезку, координату y.
// ---
double GetYFromX( const Line& line, double x )
{
  double x1 = line.GetStartPoint().GetX();
  double y1 = line.GetStartPoint().GetY();
  double x2 = line.GetEndPoint().GetX();
  double y2 = line.GetEndPoint().GetY();
  double ch = -( x1 * y2 - x2 * y1 ) - x * ( y1 - y2 );
  if ( fabs( x1 - x2 ) < CommonConstantsMath::NULL_TOL )
    return y2;
  return ch / ( x2 - x1 );
}


//-----------------------------------------------------------------------------
//  Функтор лексикографического упорядочивания отрезков: сначала отрезки лексикографически сравниваются по первой точке.
// Если первая точка у отрезков совпадает, то отрезки сравниваются по конечной точке. Считаем, что начало отрезка всегда меньше конца по координате x.
// ---
double currentX;
Point oldPoint;
struct KeySort
{

  inline bool operator () ( const LineData* lhs, const LineData* rhs ) const
  {
    double lhsY = GetYFromX( lhs->line, currentX );
    double rhsY = GetYFromX( rhs->line, currentX );
    bool is = lhsY < rhsY;
    if ( lhsY == rhsY )
    {
      if ( IsSame( lhs->line.GetStartPoint(), rhs->line.GetStartPoint() ) )
        return  GetYFromX( lhs->line, lhs->line.GetEndPoint().GetX() ) < GetYFromX( rhs->line, lhs->line.GetEndPoint().GetX() );
    }
    return ( lhsY < rhsY );
  }

};


//-----------------------------------------------------------------------------
// Проверить есть ли точка с коодинатой х в множестве всех обрабатываемых точек событий.
// ---

static bool FindAbscissa( PointEvent thePoint, std::multiset<PointEvent, IsLexLessX>& setEventPoints )
{
  for ( auto it = setEventPoints.begin(); it != setEventPoints.end(); it++ )
  {
    if ( IsEqual(it->point, thePoint.point) )
      return true;
  }
  return false;
}


//-----------------------------------------------------------------------------
// Получить для текущей точки события верхнего соседа. Если соседа нет,возвращаем false.
// ---
static bool FindNeighborUpper( LineData*& upper, std::set<LineData*, KeySort>& setProcessedSegments, std::set<LineData*, KeySort>::iterator it )
{
  if ( ++it != setProcessedSegments.end() )
  {
    upper = *it;
    return true;
  }
  else
    return false;
}


//-----------------------------------------------------------------------------
// Получить для текущей точки события нижнего соседа. Если соседа нет, возвращаем false.
// ---
static bool FindNeighborsLower( LineData*& lower, std::set<LineData*, KeySort>& setProcessedSegments, std::set<LineData*, KeySort>::iterator it )
{
  if ( --it != setProcessedSegments.end() )
  {
    lower = *it;
    return true;
  }
  else
    return false;
}


//-----------------------------------------------------------------------------
// Оператор сравнения точек пересечения.
// ---
bool  operator < ( const CurveIntersectionData obj1, const CurveIntersectionData obj2 )
{
  return ( obj1.GetParams().first < obj2.GetParams().first && obj1.GetParams().second < obj2.GetParams().second  );

}


// Set ordering occasionally could get inconsistent due to accumulated numerical errors,
// this function restores consistent ordering
void restoreSanity( std::set<LineData*, KeySort>& currentSegments )
{
  std::set<LineData*, KeySort> newSet;
  for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
    newSet.insert( *it );
  currentSegments = newSet;
}

//-----------------------------------------------------------------------------
// Обработать точку события.
// ---
static void ProcessPoint( std::multiset<PointEvent, IsLexLessX>& setEventPoints, std::set<LineData*, KeySort>& currentSegments, PointEvent point,
                   std::vector<PointEvent>& intersectionPoints,
                   std::vector<CurveIntersectionData>& params )
{
  int numPoints = setEventPoints.size();
  currentX = point.point.GetX();
  // Если точка события - левый конец отрезка - добавляем отрезок в множество отрезков, ищем верхнего и нижнего соседа этого отрезка, проверяем на пересечение.
  if ( point.type == TypeEvent::Left )
  {
    point.s1->key = point.point.GetY();
    auto it = currentSegments.insert( point.s1 ).first;

    LineData* lower = nullptr;
    std::vector<double> keys;
    for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
      keys.push_back( GetYFromX( ( *it )->line, currentX ) );
    if ( FindNeighborsLower(lower, currentSegments, it) )
    {
      Point newPoint;
      if ( (IntersectLines( lower->line, point.s1->line, newPoint) ) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        setEventPoints.insert( event );
        if ( &lower->polyline != &point.s1->polyline )
          intersectionPoints.push_back( event );
      }
      else if ( &lower->polyline != &point.s1->polyline &&
        ( fabs( Distance( lower->line, point.s1->line, newPoint ) ) <  CommonConstantsMath::PRECISION_POLYLINE ) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }   
    LineData* upper = nullptr;
    if ( FindNeighborUpper( upper, currentSegments, it ) )
    {
      Point newPoint;
      if ( IntersectLines( upper->line, point.s1->line, newPoint ) )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = point.s1;
        setEventPoints.insert( event );
        if ( &upper->polyline != &point.s1->polyline )
          intersectionPoints.push_back( event );
      }
      else if ( &upper->polyline != &point.s1->polyline  &&
        ( fabs( Distance( upper->line, point.s1->line, newPoint ) ) <  CommonConstantsMath::PRECISION_POLYLINE ) )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = point.s1;
        intersectionPoints.push_back( event );
      }
    }
  }
  else if ( point.type == TypeEvent::Right )
  {
    LineData* lower = nullptr;
    LineData* upper = nullptr;
    auto it = currentSegments.find( point.s1 );
    if ( it == currentSegments.end() )
    {
      restoreSanity( currentSegments );
      it = currentSegments.find( point.s1 );
    }
    std::vector<double> keys;
    for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
      keys.push_back( GetYFromX( ( *it )->line, currentX ) );

    if ( FindNeighborsLower( lower, currentSegments, it ) && FindNeighborUpper( upper, currentSegments, it ) )
    {
      Point newPoint;
      if ( IntersectLines( upper->line, lower->line, newPoint) )
      {
        if ( newPoint.GetX() > point.point.GetX() )
        {
          PointEvent event( newPoint, upper, TypeEvent::Intersection );
          event.s2 = lower;
          setEventPoints.insert( event );
          if ( &upper->polyline != &lower->polyline )
            intersectionPoints.push_back( event );
        }
      }
      else if ( &upper->polyline != &lower->polyline &&
        ( fabs( Distance( upper->line, lower->line, newPoint ) ) < CommonConstantsMath::PRECISION_POLYLINE )
                && newPoint.GetX() > point.point.GetX() )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }

    currentSegments.erase( it );
  }
  else
  {// Если точка является точкой пересечения.
    currentX = 0.5 * ( oldPoint.GetX() + point.point.GetX() );
    std::vector<double> keys;
    for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
      keys.push_back( GetYFromX((*it)->line, currentX) );
    LineData* s1 = point.s1;
    auto itS1 = currentSegments.find( point.s1 );
    LineData* s2 = point.s2;
    auto itS2 = currentSegments.find( point.s2 );
    if ( itS1 == currentSegments.end() || itS2 == currentSegments.end() )
    {
      restoreSanity( currentSegments );
      itS1 = currentSegments.find( point.s1 );
      itS2 = currentSegments.find( point.s2 );
    }
    LineData* lower;
    LineData* upper;
    if ( FindNeighborUpper( upper, currentSegments, itS1 ) && upper !=s1 )
    {
      Point newPoint;
      if (  ( IntersectLines( upper->line, point.s2->line, newPoint ) ) )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = point.s2;
        setEventPoints.insert( event );
        intersectionPoints.push_back( event );
      }
      else if ( std::abs( Distance( upper->line, s2->line, newPoint ) < CommonConstantsMath::PRECISION_POLYLINE ) && upper != s1 )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = point.s2;
        intersectionPoints.push_back( event );

      }

    }
    if ( FindNeighborsLower(lower, currentSegments, itS2))
    {
      Point newPoint;
      if (  ( IntersectLines( lower->line, point.s1->line, newPoint )) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        setEventPoints.insert( event );
        intersectionPoints.push_back( event );
      }
      else if ( lower != s2 && std::abs( Distance( lower->line, s1->line, newPoint ) < 2. *CommonConstantsMath::PRECISION_POLYLINE ) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );

      }
    }
    if ( itS1 != currentSegments.end() && itS2 != currentSegments.end() && itS1 != itS2 )
    {
      s1 = *itS1;     
      s2 = *itS2;
      currentSegments.erase( itS1 );
      currentSegments.erase( currentSegments.find(point.s2 ) );
      auto it = setEventPoints.begin();
      while ( it->point.GetX() <= point.point.GetX() )
        it++;
      double shiftX = it->point.GetX();
      currentX = 0.5 * ( point.point.GetX() + shiftX );
      currentSegments.insert( s1 );
      currentSegments.insert( s2 );
    }

  }
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
    double s1Param = 0.0;
    double s2Param = 0.0;
    if ( intersectionPoints[i].s1->curve.GetType() == Curve::CurveType::LineType )
      s1Param = static_cast< Line* >( &intersectionPoints[i].s1->curve )->GetTFromPoint( intersectionPoints[i].point );
    else
      s1Param = intersectionPoints[i].s1->leftParam;
    if ( intersectionPoints[i].s2->curve.GetType() == Curve::CurveType::LineType )
      s2Param = static_cast< Line* >( &intersectionPoints[i].s2->curve )->GetTFromPoint( intersectionPoints[i].point );
    else
      s2Param = intersectionPoints[i].s2->leftParam;

    if ( intersectionPoints[i].s1->curve.GetType() == Curve::CurveType::PolylineType )
      s1Param = static_cast< GeomPolyline* >( &intersectionPoints[i].s1->curve )->GetTFromPoint( intersectionPoints[i].point );
    else
      s1Param = intersectionPoints[i].s1->leftParam;
    if ( intersectionPoints[i].s2->curve.GetType() ==  Curve::CurveType::PolylineType)
      s2Param = static_cast< GeomPolyline* >( &intersectionPoints[i].s2->curve )->GetTFromPoint( intersectionPoints[i].point );
    else
      s2Param = intersectionPoints[i].s2->leftParam;
      params.push_back( CurveIntersectionData( intersectionPoints[i].s1->curve, intersectionPoints[i].s2->curve,
                                               std::make_pair( s1Param, s2Param )) );
  }
  intersectionPoints.clear();
  if ( oldPoint.GetX() < point.point.GetX() )
    oldPoint = point.point;
}


//-----------------------------------------------------------------------------
//  Добавить в множество точек событий опорные точки полилинии.
// ---
static void CollectEventPoints( std::vector<LineData>& lines, const Math::GeomPolyline& polyline, Curve& curves,
                         std::multiset<PointEvent, IsLexLessX>& setEventPoints, size_t numPolyline )
{
  std::vector<Point> polylinePoints = polyline.GetReferensedPoints();
  std::vector<double> refParams = polyline.GetReferensedParams();
  size_t start = lines.size();
  Point startPoint;
  Point endPoint;
  for ( size_t i = 0; i < polylinePoints.size() - 1; ++i )
  {
   
    if ( polylinePoints[i].GetX() < polylinePoints[i + 1].GetX() )
    {
      startPoint = polylinePoints[i];
      endPoint = polylinePoints[i + 1];
    }
    else
    {
      startPoint = polylinePoints[i + 1];
      endPoint = polylinePoints[i];
    }

    double urand1 = (double)rand() / ( double ) RAND_MAX;
    double urand2 = ( double ) rand() / ( double ) RAND_MAX;
    double urand3 = ( double ) rand() / ( double ) RAND_MAX;
    double urand4 = ( double ) rand() / ( double ) RAND_MAX;
    double lenght = ( startPoint - endPoint ).Lenght() * CommonConstantsMath::SCALING;
    Point newStart = startPoint + Point( ( urand1 - 0.5 )*lenght, ( urand2 - 0.5 ) * lenght );
    Point newEnd = endPoint + Point( ( urand3 - 0.5 )*lenght, ( urand4 - 0.5 ) * lenght );
    lines.push_back( LineData(Line( newStart, newEnd ), polyline, refParams[i], numPolyline, 0., curves) );

  }
  for ( size_t i = start; i < lines.size(); ++i )
  { 
    setEventPoints.insert( PointEvent( lines[i].line.GetStartPoint(), &lines[i], TypeEvent::Left ) );
    setEventPoints.insert( PointEvent( lines[i].line.GetEndPoint(), &lines[i], TypeEvent::Right ) );
  }

}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезков полилилинй. В параметр params записываются параметры исходных кривых, соответсвующих пересекающимся отрезкам.
// ---
static void SegmentsIntersections( std::vector< Math::GeomPolyline>& polyline,
                                          std::vector<CurveIntersectionData>& params,
                                          std::vector<Curve*> curves )
{
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, IsLexLessX> setEventPoints;
  std::set<LineData*, KeySort> T;
  size_t size = 0;
  for ( size_t i = 0; i < polyline.size(); i++ )
    size += polyline[i].GetReferensedPoints().size();
  std::vector<LineData> lines;
  lines.reserve( size );
  for ( size_t i = 0; i < polyline.size(); i++ )
  {
    if ( curves[i] != nullptr )
    CollectEventPoints( lines, polyline[i], *curves[i], setEventPoints, i );
  }
  oldPoint = setEventPoints.begin()->point;
  while ( !setEventPoints.empty() )
  {
    PointEvent currentPoint = *setEventPoints.begin();

    setEventPoints.erase( setEventPoints.begin() );
    ProcessPoint( setEventPoints, T, currentPoint, intersectPoints, params );
  }
}


//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых, используя алгоритм перечения отрзков из Preparata,Sheimos.
// ---
std::vector<CurveIntersectionData> Intersect( const std::vector<Curve*>& curves )
{
  std::vector<GeomPolyline> polylinePoints( curves.size() );
  for ( size_t i = 0; i < curves.size(); i++ )
  {
    if ( curves[i] != nullptr )
    {
      curves[i]->GetAsPolyLine( polylinePoints[i] );
    }
  }

  std::vector<CurveIntersectionData> intersections;
  SegmentsIntersections( polylinePoints, intersections, curves );
  std::vector<CurveIntersectionData> intersectPoints;

  for ( size_t i = 0; i < intersections.size(); i++ )
  {
    std::pair<double,double> intersectPoint = NewtonMethod( intersections[i].GetCurve1(), intersections[i].GetCurve2(), intersections[i].GetParams() );

    if ( Distance(intersections[i].GetCurve1().GetPoint(intersectPoint.first), intersections[i].GetCurve2().GetPoint(intersectPoint.second) ) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( CurveIntersectionData( intersections[i].GetCurve1(), intersections[i].GetCurve2(), intersectPoint) );
  }

  return intersectPoints;
}


///////////////////////////////////////////////////////////////////////////////
//
/// Приложение. Функции, не используемые в основном алгоритме.
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//  Найти пересечение отрезка и окружности.
// ---
static std::vector<Point> IntersectLineCircle( const Curve& line, const Curve& circle )
{
  std::vector<Point> result;
  const Math::Line* currentLine = dynamic_cast< const Math::Line* >( &line );
  const Math::Ellipse* currentCircle = dynamic_cast< const Math::Ellipse* >( &circle );
  Point p1 = currentLine->GetStartPoint();
  Point p2 = currentLine->GetEndPoint();
  Point sc = currentCircle->GetCenter();
  double r = currentCircle->GetMajorRadius();

  Vector dp = p2 - p1;

  double a = dp * dp;
  double b = 2. * ( dp.GetX() * ( p1.GetX() - sc.GetX() ) + dp.GetY() * ( p1.GetY() - sc.GetY() ) );
  double c = sc * sc;
  c += p1 * p1;
  c -= 2. * ( sc * p1 );
  c -= r * r;
  double bb4ac = b * b - 4 * a * c;
  if ( fabs( a ) < CommonConstantsMath::NULL_TOL || bb4ac < 0 )
  {
    return result;
  }

  const double mu1 = ( -b + sqrt( bb4ac ) ) / ( 2. * a );
  const double mu2 = ( -b - sqrt( bb4ac ) ) / ( 2. * a );
  if ( mu1 == mu2 )
  {
    result.push_back( p1 + ( p2 - p1 ) * mu1 );
    return result;
  }
  else
  {
    Point point1 = p1 + ( p2 - p1 ) * mu1;
    if ( IsYinSegment( *currentLine, point1.GetY() ) && IsXinSegment( *currentLine, point1.GetX() ) )
      result.push_back( point1 );
    Point point2 = p1 + ( p2 - p1 ) * mu2;

    if ( IsYinSegment( *currentLine, point2.GetY() ) && IsXinSegment( *currentLine, point2.GetX() ) )
      result.push_back( point2 );
    return result;
  }
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух полилиний. Это тривиальный случай: перебираем попарно все полилинии, сравниваем отреки.
// Необходим для подтверждения корректности быстрого алгоритма.
// ---
static std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2, std::vector<CurveIntersectionData>& resultParams )
{
  std::vector<Point> intersectPoints;
  const Math::GeomPolyline* lineCurveFirst = dynamic_cast< const Math::GeomPolyline* >( &curve1 );
  std::vector<Point> refPointsFirst = lineCurveFirst->GetReferensedPoints();
  const Math::GeomPolyline* lineCurveSecound = dynamic_cast< const Math::GeomPolyline* >( &curve2 );

  std::vector<double> refParams1 = lineCurveFirst->GetReferensedParams();
  std::vector<double> refParams2 = lineCurveSecound->GetReferensedParams();

  std::vector<Point> refPointsSecound = lineCurveSecound->GetReferensedPoints();
  for ( size_t i = 1; i < refPointsFirst.size(); ++i )
  {
    Line firstLine( refPointsFirst[i - 1], refPointsFirst[i] );
    for ( size_t j = 1; j < refPointsSecound.size(); ++j )
    {
      Point point;
      if ( IntersectLines( Line( refPointsSecound[j - 1], refPointsSecound[j] ), firstLine, point ) )
      {
        auto result = std::find( std::begin( intersectPoints ), std::end( intersectPoints ), point );
        if ( result == std::end( intersectPoints ) )
        {
          intersectPoints.push_back( point );
          resultParams.push_back( CurveIntersectionData( curve1, curve2, std::make_pair( refParams1[i], refParams2[j] ) ) );
        }
      }
    }
  }
  std::set<Point> s( intersectPoints.begin(), intersectPoints.end() );
  intersectPoints.assign( s.begin(), s.end() );
  return intersectPoints;
}




}
