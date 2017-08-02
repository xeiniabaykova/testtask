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
namespace Math {

///////////////////////////////////////////////////////////////////////////////
//
/// Метод Ньютона и вспомогательные функции.
///////////////////////////////////////////////////////////////////////////////

using matrix22 = std::array<std::array<double, 2>, 2>;
//-----------------------------------------------------------------------------
//  Найти значение матрицы Гессе для квадрата расстояния между кривыми в точке (t1, t2).
// ---
static matrix22 CountingHessian( const Curve* curve1, const Curve* curve2, double t1, double t2 )
{
  const Point t1Point = curve1->GetPoint( t1 );
  const double x1 = t1Point.GetX();
  const double y1 = t1Point.GetY();

  const Point t1PointD = curve1->GetDerivative( t1 );
  const double x1d = t1PointD.GetX();
  const double y1d = t1PointD.GetY();

  const Point t1Point2D = curve1->Get2Derivative( t1 );
  const double x1dd = t1Point2D.GetX();
  const double y1dd = t1Point2D.GetY();

  const Point t2Point = curve2->GetPoint( t2 );
  const double x2 = t2Point.GetX();
  const double y2 = t2Point.GetY();

  const Point t2PointD = curve2->GetDerivative( t2 );
  const double x2d = t2PointD.GetX();
  const double y2d = t2PointD.GetY();

  const Point t2Point2D = curve2->Get2Derivative( t2 );
  const double x2dd = curve2->Get2Derivative( t2 ).GetX();
  const double y2dd = curve2->Get2Derivative( t2 ).GetY();

  const double F11 = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  const double F12 = -2. * x2d * x1d - 2. * y2d * y1d;
  const double F21 = -2. *x1d * x2d - 2. * y1d * y2d;
  const double F22 = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;

  matrix22 hessian;
  hessian[0][0] = F11;
  hessian[0][1] = F12;
  hessian[1][0] = F21;
  hessian[1][1] = F22;
  const double determinant = hessian[0][0] * hessian[1][1] - hessian[0][1] * hessian[1][0];
  if ( fabs( determinant ) < CommonConstantsMath::NULL_TOL )
  {
    hessian[0][0] = 1.;
    hessian[0][1] = 0;
    hessian[1][0] = 0;
    hessian[1][1] = 1.;
  }
  return std::move( hessian );
}


//-----------------------------------------------------------------------------
//  Вернуть обратную матрицу. Предполагается, что матрица невырождена.
// ---
static matrix22& InverseMatrix( matrix22& matrix )
{
  matrix22 result( matrix );
  const double determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  result[0][0] = result[1][1] / determinant;
  result[0][1] = -result[1][0] / determinant;
  result[1][0] = -matrix[0][1] / determinant;
  result[1][1] = matrix[0][0] / determinant;
  return std::move( result );
}


//-----------------------------------------------------------------------------
//  Вернуть градиент для квадрата расстояния между кривыми в заданной точке.
// ---
static Vector Gradient( const Curve* curve1, const Curve* curve2, std::pair<double, double> paramsCurve )
{
  Point point1 = curve1->GetPoint( paramsCurve.first );
  Vector grad1 = curve1->GetDerivative( paramsCurve.first );

  Point point2 = curve2->GetPoint( paramsCurve.second );
  Vector grad2 = curve2->GetDerivative( paramsCurve.second );
  double aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  double aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
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
//  Провести одну итерацию метода Ньютона.
// ---
static std::pair<double, double> NewtonMethodIteration( const Curve* curve1, const Curve* curve2, std::pair<double, double> paramsCurve )
{

  auto invHessian = CountingHessian( curve1, curve2, paramsCurve.first, paramsCurve.second );
  invHessian = InverseMatrix( invHessian );
  const Vector grad = Gradient( curve1, curve2, paramsCurve );
  std::pair<double, double> step( invHessian[0][0] * -grad.GetX() + invHessian[0][1] * -grad.GetY(), invHessian[1][0] *
                                  -grad.GetX() + invHessian[1][1] * -grad.GetY() );
  return paramsCurve + step;
}


//-----------------------------------------------------------------------------
//  Запустить метод Ньютона для поиска локального минимума квадарта расстояния между кривыми с начальной точкой в startValue.
// ---
static std::pair<double, double> NewtonMethod( const Curve* curve1, const Curve* curve2, std::pair<double, double> startValue )
{
  std::pair<double, double> currentPoint = startValue;
  std::pair<double, double> newPoint;
  for ( size_t i = 0; i < CommonConstantsMath::NUMBER_NEWTON_METHOD; i++ )
  {
    newPoint = NewtonMethodIteration( curve1, curve2, currentPoint );
    if ( fabs( Distance( newPoint, currentPoint ) ) < CommonConstantsMath::ACCURANCY_METHOD_NEWTON )
      break;
    currentPoint = newPoint;
  }
  return newPoint;
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
  const Vector dir1 = lineCurveFirst.GetDerivative( 0.);
  const Vector dir2 = lineCurveSecond.GetDerivative( 0. );
  const Point start1 = lineCurveFirst.GetStartPoint();
  const Point start2 = lineCurveSecond.GetStartPoint();
  const Point end1 = lineCurveFirst.GetEndPoint();
  const Point end2 = lineCurveSecond.GetEndPoint();

  if ( !dir1.IsCollinear( dir2 ) )
  {
    const double StartPointDifferenceX = start1.GetX() - start2.GetX();
    const double StartPointDifferenceY = start1.GetY() - start2.GetY();
    double t1Intersect = ( StartPointDifferenceY - dir2.GetY() / dir2.GetX() * StartPointDifferenceX ) / ( dir2.GetY() * dir1.GetX() / dir2.GetX() - dir1.GetY() );
    double t2Intersect = ( StartPointDifferenceX + dir1.GetX() * t1Intersect ) / dir2.GetX();
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
  Line                      line;     // Отрезок.
  const GeomPolyline*       polyline; // Полилиния, которой принадлежит отрезок.
  size_t                    numParam; // Номер полилинии в списке полилиний.
  Curve                     *curve; // Кривая, соответствующая отрезку.
  double                    leftParam; // Параметр исходной кривой, соответсующий отрезку полилинии.
  double                    key;      // Ключ для сравнения отрезков - по умолчанию координата y левой точки.
  LineData    ( Line theLine, const GeomPolyline* thePolyline, double theLeftParam, size_t thenumParam, double theKey, Curve* theCurve ):
    line      ( theLine ),
    polyline  ( thePolyline ),
    leftParam ( theLeftParam ),
    numParam  ( thenumParam ),
    key       ( theKey ),
    curve     ( theCurve )
  {}
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
static bool IsLexLess( const Point& lhs, const Point& rhs )
{
  if ( IsSame( lhs, rhs ) )
    return ( lhs.GetY() < rhs.GetY() );
  else
    return ( lhs.GetX() < rhs.GetX() );
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
      return IsLexLess( lhs.s1->line.GetEndPoint(), rhs.s1->line.GetEndPoint() );
    }
    return IsLexLess( lhs.point, rhs.point );
  }
};


//-----------------------------------------------------------------------------
//  Функтор лексикографического упорядочивания отрезков: сначала отрезки лексикографически сравниваются по первой точке.
// Если первая точка у отрезков совпадает, то отрезки сравниваются по конечной точке. Считаем, что начало отрезка всегда меньше конца по координате x.
// ---
struct KeySort
{
  inline bool operator () ( const LineData* lhs, const LineData* rhs ) const
  {
    /*  если один ключ меньше другого.возвращаем true
    иначе если они равны, то возвращаем результат сравнения точек.
    иначе возвращаем false*/

    /* если первая точка первого отрезка лексикографически меньше
    первой точки второго, то вернуть true, иначе если они равны то вернуть, меньше ли вторая точка первого отрезхка второй точки второго отрезка*/

    if ( lhs->key < rhs->key )
      return true;
    else if ( lhs->key == rhs->key )
    {
      if ( IsLexLess(lhs->line.GetStartPoint(), rhs->line.GetStartPoint()) )
        return true;
      else if ( IsLexLess(rhs->line.GetStartPoint(), lhs->line.GetStartPoint()) )
        return false;
      else
        return IsLexLess( lhs->line.GetEndPoint(), rhs->line.GetEndPoint() );
    }
    else
      return false;
   /* свести к четырем точкам*/
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
// Обработать точку события.
// ---
static void ProcessPoint( std::multiset<PointEvent, IsLexLessX>& setEventPoints, std::set<LineData*, KeySort>& T, PointEvent point,
                   std::vector<PointEvent>& intersectionPoints,
                   std::vector<CurveIntersectionData>& params )
{
  // Если точка события - левый конец отрезка - добавляем отрезок в множество отрезков, ищем верхнего и нижнего соседа этого отрезка, проверяем на пересечение.
  if ( point.type == TypeEvent::Left )
  {
    point.s1->key = point.point.GetY();
    auto it = T.insert( point.s1 ).first;

    LineData* lower = nullptr;
    if ( FindNeighborsLower(lower, T, it) )
    {
      Point newPoint;
      if ( lower->polyline != point.s1->polyline && (IntersectLines( lower->line, point.s1->line, newPoint ) ||
                                                       fabs(Distance( lower->line, point.s1->line, newPoint) < 2. * CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }   
    LineData* upper = nullptr;
    if ( FindNeighborUpper( upper, T, it ) )
    {
      Point newPoint;
      if ( upper->polyline != point.s1->polyline && ( IntersectLines( upper->line, point.s1->line, newPoint ) ||
                                                      (fabs (Distance(upper->line, point.s1->line, newPoint))  < 2. * CommonConstantsMath::PRECISION_POLYLINE)) )
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
    auto it = T.find( point.s1 );
    if ( FindNeighborsLower( lower, T, it ) && FindNeighborUpper( upper, T, it ) )
    {
      Point newPoint;
      if ( lower->polyline != upper->polyline && ( IntersectLines( upper->line, lower->line, newPoint ) ||
                                                   fabs(Distance( upper->line,lower->line, newPoint) < 2. * CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        if ( newPoint.GetX() > point.point.GetX() )
        {
          PointEvent event( newPoint, upper, TypeEvent::Intersection );
          event.s2 = lower;
          intersectionPoints.push_back( event );
        }
      }
    }

    T.erase( it );
  }
  else
  {// Если точка является точкой пересечения.
    LineData* s1 = point.s1;
    auto itS1 = T.find( point.s1 );
    LineData* s2 = point.s2;
    auto itS2 = T.find( point.s2 );
    LineData* lower;
    LineData* upper;
    if ( FindNeighborUpper( upper, T, itS1 )/* && upper != s2*/ )
    {
      Point newPoint;
      if ( upper->polyline != s2->polyline && ( IntersectLines( upper->line, point.s2->line, newPoint ) ||
                                                 fabs(Distance(upper->line, s2->line, newPoint) < 2. * CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        PointEvent event( newPoint, upper, TypeEvent::Intersection );
        event.s2 = point.s2;
        intersectionPoints.push_back( event );
      }
    }
    if ( FindNeighborsLower(lower, T, itS2) && lower != s1 )
    {
      Point newPoint;
      if ( lower->polyline != s1->polyline && ( IntersectLines( lower->line, point.s1->line, newPoint ) ||
                                                 fabs(Distance(lower->line, s1->line, newPoint) < 2. *CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }
    if ( itS1 != T.end() && itS2 != T.end() && itS1 != itS2 )
    {
      s1 = *itS1;     
      s2 = *itS2;
      T.erase( itS1 );
      T.erase(T.find(point.s2 ) );
      double key = s1->key;
      s1->key = s2->key;
      s2->key = key;
      T.insert( s1 );
      T.insert( s2 );
    }

  }
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
      params.push_back( CurveIntersectionData( intersectionPoints[i].s1->curve, intersectionPoints[i].s2->curve,
                                               std::make_pair( intersectionPoints[i].s1->leftParam, intersectionPoints[i].s2->leftParam)) );
    if ( !FindAbscissa( intersectionPoints[i], setEventPoints ) )
    {
      setEventPoints.insert( intersectionPoints[i] );
    }
  }
  intersectionPoints.clear();
}


//-----------------------------------------------------------------------------
//  Добавить в множество точек событий опорные точки полилинии.
// ---
static void CollectEventPoints( std::vector<LineData>& lines, const Math::GeomPolyline* polyline, Curve* curves,
                         std::multiset<PointEvent, IsLexLessX>& setEventPoints, size_t numPolyline )
{
  std::vector<Point> polylinePoints = polyline->GetReferensedPoints();
  std::vector<double> refParams = polyline->GetReferensedParams();
  size_t start = lines.size();
  for ( size_t i = 0; i < polylinePoints.size() - 1; ++i )
  {
    if ( polylinePoints[i].GetX() < polylinePoints[i + 1].GetX() )
    {
      lines.push_back( LineData(Line( polylinePoints[i], polylinePoints[i + 1] ), polyline, refParams[i], numPolyline, 0., curves) );
    }
    else
    {
      lines.push_back( LineData(Line( polylinePoints[i + 1], polylinePoints[i] ), polyline, refParams[i], numPolyline, 0., curves) );
    }
  }
  for ( size_t i = start; i < lines.size(); ++i )
  {
    // Искусственное масштабирование для правильной обработки пересечений на концах сегмента.
    Point newStart = lines[i].line.GetStartPoint() + ( lines[i].line.GetStartPoint() - lines[i].line.GetEndPoint() ) * CommonConstantsMath::SCALING;
    Point newEnd = lines[i].line.GetEndPoint() + ( lines[i].line.GetEndPoint() - lines[i].line.GetStartPoint() ) * CommonConstantsMath::SCALING;
    setEventPoints.insert( PointEvent( newStart, &lines[i], TypeEvent::Left ) );
    setEventPoints.insert( PointEvent( newEnd + Point(CommonConstantsMath::NULL_TOL, CommonConstantsMath::NULL_TOL), &lines[i], TypeEvent::Right ) );
  }

}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезков полилилинй. В параметр params записываются параметры исходных кривых, соответсвующих пересекающимся отрезкам.
// ---
static void SegmentsIntersections( std::vector< Math::GeomPolyline*> polyline,
                                          std::vector<CurveIntersectionData>& params,
                                          std::vector<Curve*> curves )
{
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, IsLexLessX> setEventPoints;
  std::set<LineData*, KeySort> T;
  std::vector<LineData> lines;
  size_t size = 0;
  for ( size_t i = 0; i < polyline.size(); i++ )
    size += polyline[i]->GetReferensedPoints().size();
  lines.reserve( size );
  for ( size_t i = 0; i < polyline.size(); i++ )
  {
    if ( curves[i] != nullptr )
    CollectEventPoints( lines, polyline[i], curves[i], setEventPoints, i );
  }

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
  std::vector<GeomPolyline*> polylinePoints( curves.size() );
  for ( size_t i = 0; i < curves.size(); i++ )
  {
    if ( curves[i] != nullptr )
    {
      polylinePoints[i] = new GeomPolyline();
      curves[i]->GetAsPolyLine( *polylinePoints[i] );
    }
  }

  std::vector<CurveIntersectionData> intersections;
  SegmentsIntersections( polylinePoints, intersections, curves );
  std::vector<CurveIntersectionData> intersectPoints;

  for ( size_t i = 0; i < intersections.size(); i++ )
  {
    std::pair<double,double> intersectPoint = NewtonMethod( intersections[i].curve1, intersections[i].curve2, intersections[i].GetParamPoint() );

    if ( Distance(intersections[i].curve1->GetPoint(intersectPoint.first), intersections[i].curve2->GetPoint(intersectPoint.second) ) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( CurveIntersectionData( intersections[i].curve1, intersections[i].curve2, intersectPoint) );
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
          resultParams.push_back( CurveIntersectionData( &curve1, &curve2, std::make_pair( refParams1[i], refParams2[j] ) ) );
        }
      }
    }
  }
  std::set<Point> s( intersectPoints.begin(), intersectPoints.end() );
  intersectPoints.assign( s.begin(), s.end() );
  return intersectPoints;
}




}
