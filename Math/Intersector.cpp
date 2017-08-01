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
namespace Math {
namespace {

  //-----------------------------------------------------------------------------
  //  Найти значение матрицы Гессе для квадрата расстояния между кривыми в точке (t1, t2).
  // ---
std::vector<std::vector<double>> CountingHessian ( const Curve& curve1, const Curve& curve2, double t1, double t2 )
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

  const double F11 = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  const double F12 = -2. * x2d * x1d - 2. * y2d * y1d;
  const double F21 = -2. *x1d * x2d - 2. * y1d * y2d;
  const double F22 = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;

  std::vector<std::vector <double>> hessian( 2, std::vector<double>(2) );
  hessian[0][0] = F11;
  hessian[0][1] = F12;
  hessian[1][0] = F21;
  hessian[1][1] = F22;
  const double determinant = hessian[0][0] * hessian[1][1] - hessian[0][1] * hessian[1][0];
  if ( fabs(determinant) < CommonConstantsMath::NULL_TOL )
  {
    hessian[0][0] = 1.;
    hessian[0][1] = 0;
    hessian[1][0] = 0;
    hessian[1][1] = 1.;
  }
  return hessian;
}


//-----------------------------------------------------------------------------
//  Вернуть обратную матрицу. Предполагается, что матрица невырождена.
// ---
std::vector<std::vector<double>> InverseMatrix( const std::vector<std::vector<double>>& matrix )
{
  std::vector<std::vector<double>> result( 2, std::vector<double> (2) );
  const double determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  result[0][0] = matrix[1][1] / determinant;
  result[0][1] = -matrix[1][0] / determinant;
  result[1][0] = -matrix[0][1] / determinant;
  result[1][1] = matrix[0][0] / determinant;
  return result;
}


//-----------------------------------------------------------------------------
//  Вернуть градиент для квадрата расстояния между кривыми в заданной точке.
// ---
Vector Gradient( const Curve& curve1, const Curve& curve2, Point point )
{
  Point point1 = curve1.GetPoint ( point.GetX() );
  Vector grad1 = curve1.GetDerivative ( point.GetX() );

  Point point2 = curve2.GetPoint ( point.GetY() );
  Vector grad2 = curve2.GetDerivative ( point.GetY() );
  double aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  double aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
  return Vector ( aResultT1, aResultT2 );
}


//-----------------------------------------------------------------------------
//  Провести одну итерацию метода Ньютона.
// ---
Point NewtonMethodIteration( const Curve& curve1, const Curve& curve2, Point start )
{
  auto invHessian = InverseMatrix( CountingHessian( curve1, curve2, start.GetX(), start.GetY() ) );
  const Vector grad = Gradient( curve1, curve2, start );
  const Point step( invHessian[0][0] * -grad.GetX() + invHessian[0][1] * -grad.GetY(), invHessian[1][0] *
                    -grad.GetX() + invHessian[1][1] * -grad.GetY() );
  return start + step;
}


//-----------------------------------------------------------------------------
// Проверить, содержит ли отрезок точку с координатой х.
// ---
bool IsXinSegment( const Line& line, double x )
{
  return ( x - line.GetStartPoint().GetX() ) * ( line.GetEndPoint().GetX() - x ) >= 0;
}


//-----------------------------------------------------------------------------
// Проверить, содержит ли отрезок точку с координатой y.
// ---
bool IsYinSegment( const Line& line, double y )
{
  return ( y - line.GetStartPoint().GetY() ) * ( line.GetEndPoint().GetY() - y ) >= 0;
}


//-----------------------------------------------------------------------------
//  Проверить принадлежность текущей точки отрезку. 
// ---
bool ISPointInSegment( const Point& testPoint, const Line& line )
{
  const double a = line.GetEndPoint().GetY() - line.GetStartPoint().GetY();
  const double b = line.GetStartPoint().GetX() - line.GetEndPoint().GetX();
  const double c = -a * line.GetStartPoint().GetX() - b * line.GetStartPoint().GetY();
  if ( fabs( a * testPoint.GetX() + b * testPoint.GetY() + c ) > CommonConstantsMath::NULL_TOL )
    return false;

  return IsYinSegment( line, testPoint.GetY() ) && IsYinSegment( line, testPoint.GetX() );
}
}

//-----------------------------------------------------------------------------
//  Найти пересечение полилинии и окружности.
// ---
std::vector<Point> IntersectPolylineCircle ( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение полилинии и эллипса.
// ---
std::vector<Point> IntersectPolylineEllipse ( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух отрезков.
// ---
bool IntersectLines( const Curve& curve1, const Curve& curve2, Point& thePoint )
{
  std::vector<Point> IntersectPoints;
  const Math::Line* lineCurveFirst = dynamic_cast< const Math::Line* >( &curve1 );
  const Math::Line* lineCurveSecound = dynamic_cast< const Math::Line* >( &curve2 );

  const Vector dir1 = lineCurveFirst->GetEndPoint() - lineCurveFirst->GetStartPoint();
  const Vector dir2 = lineCurveSecound->GetEndPoint() - lineCurveSecound->GetStartPoint();
  const Point start1 = lineCurveFirst->GetStartPoint();
  const Point start2 = lineCurveSecound->GetStartPoint();
  const Point end1 = lineCurveFirst->GetEndPoint();
  const Point end2 = lineCurveSecound->GetEndPoint();

  if ( !dir1.IsCollinear( dir2 ) )
  {
    //считаем уравнения прямых проходящих через отрезки
    const double a1 = -dir1.GetY();
    const double b1 = dir1.GetX();
    const double d1 = -( a1 * start1.GetX() + b1 * start1.GetY() );

    const double a2 = -dir2.GetY();
    const double b2 = +dir2.GetX();
    const double d2 = -( a2 * start2.GetX() + b2 * start2.GetY() );

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    const double seg1Line2Start = a2 * start1.GetX() + b2 * start1.GetY() + d2;
    const double seg1Line2End = a2 * end1.GetX() + b2*end1.GetY() + d2;

    const double seg2Line1Start = a1 * start2.GetX() + b1 * start2.GetY() + d1;
    const double seg2Line1End = a1 * end2.GetX() + b1 * end2.GetY() + d1;

    //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
    if ( seg1Line2Start * seg1Line2End > 0 || seg2Line1Start * seg2Line1End > 0 )
      return false;

    double u = seg1Line2Start / ( seg1Line2Start - seg1Line2End );
    thePoint = start1 + dir1 * u;

    return true;
  }
  else
  {
    bool isInOneLine = ( Vector( start1 - start2 ).IsCollinear( Vector( start2 - end1 ) ) ) && ( Vector( start1 - start2 ).IsCollinear( Vector( start2 - end2 ) ) );
    if ( isInOneLine )
    {
      if ( IsEqual( start1, end2 ) )
      {
        thePoint = end2;
        return true;
      }
      if ( IsEqual( start2, end1 ) )
      {
        thePoint = end1;
        return true;
      }
      if ( IsEqual( end2, end1 ) )
      {
        thePoint = end1;
        return true;
      }
      if ( IsEqual( start1, start2 ) )
      {
        thePoint = start1;
        return true;
      }
      return false;
    }
    return false;
  }

}



//-----------------------------------------------------------------------------
//  Перечисление для определения типа события точки:
// Точка может быть левым концом отрезка, правым концом отрезка, точкой перечечения, невалидной точкой.
// ---
enum typeEvent
{
  Intersection, // Точка пересечения отрезков.
  Left,         // Левый конец отрезка.
  Right        // Правый конец отрезка.
};


//-----------------------------------------------------------------------------
//  Отрезок и данные, необходимые для нахзождения пересечения отрезков. Так же хрнится указатель на полилинию.
//  это необходимо, чтобы не учитывать перечечения отрезков внутри полилинии.
// ---
struct LineData
{
  Line                      line;     // Отрезок.
  const GeomPolyline*       polyline; // Полилиния, которой принадлежит отрезок.
  size_t                    numParam; // Номер полилинии в списке полилиний.
  double                    leftParam; // Параметр исходной кривой, соответсующий отрезку полилинии.
  double                    key;      // Ключ для сравнения отрезков - по умолчанию координата y левой точки.
  LineData ( Line theLine, const GeomPolyline* thePolyline, double theLeftParam, size_t thenumParam, double theKey ):
    line ( theLine ),
    polyline ( thePolyline ),
    leftParam ( theLeftParam ),
    numParam ( thenumParam ),
    key ( theKey )
  {}
};


//-----------------------------------------------------------------------------
//  Вспомогательная структура для алгоритма быстрого пересечения отрезков.
// Хранится точка текущего события, тип события, номер сегмента (возможно, больше не нужно)
// в случае, гогда тип события пересечение - хранятся отрезки.
// ---
struct PointEvent
{
  typeEvent type; // Тип точки события.
  Point     point; // Точка события.
  LineData  *s1;   // Отрезок, соответствующий точке события.
  LineData  *s2;   // Если точка события является точкой пересечения, то храним второй отрезок тоже.
  PointEvent( Point thePoint, LineData *theS1, typeEvent theType ):
    point( thePoint ),
    type( theType ),
    s1( theS1 )
  {}
};


//-----------------------------------------------------------------------------
// Сравнить точки лексикографически.
// ---
bool IsLexLess( const Point& lhs, const Point& rhs )
{
  if ( lhs.GetX() > rhs.GetX() )
    return false;
  if ( lhs.GetX() < rhs.GetX() )
    return true;
  return ( lhs.GetY() < rhs.GetY() );
}


//-----------------------------------------------------------------------------
// Проверить точки на равенство.
// ---
bool IsSame( const Point& lhs, const Point& rhs )
{

  return ( lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY() );
}

//-----------------------------------------------------------------------------
//  Функтор лексиграфического упорядочивания точек по возрастанию.
// ---
struct SortByX
{
  inline bool operator()( const PointEvent &lhs, const PointEvent& rhs ) const
  {
    if ( IsLexLess( lhs.point, rhs.point ) )
      return true;
    else if ( IsSame(lhs.point, rhs.point) )
    {
      return IsLexLess( lhs.s1->line.GetEndPoint(), rhs.s1->line.GetEndPoint() );
    }
    return false;
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
  }
};


//-----------------------------------------------------------------------------
// Проверить есть ли точка с коодинатой х в множестве всех обрабатываемых точек событий.
// ---
bool FindAbscissa( PointEvent thePoint, std::multiset<PointEvent, SortByX>& Q )
{
  for ( auto it = Q.begin(); it != Q.end(); it++ )
  {
    if ( IsEqual(it->point, thePoint.point) )
      return true;
  }
  return false;
}


//-----------------------------------------------------------------------------
// Получить для текущей точки события верхнего соседа. Если соседа нет,возвращаем false.
// ---
bool FindNeighborUpper( LineData*& upper, std::set<LineData*, KeySort>& T, const std::set<LineData*, KeySort>::iterator it )
{
  auto plusIt = it;
  plusIt++;
  if ( plusIt != T.end() )
  {
    upper = *plusIt;
    return true;
  }
  else
    return false;
}


//-----------------------------------------------------------------------------
// Получить для текущей точки события нижнего соседа. Если соседа нет, возвращаем false.
// ---
bool FindNeighborsLower( LineData*& lower, std::set<LineData*, KeySort>& T, const std::set<LineData*, KeySort>::iterator it )
{
  auto minusIt = it;
  minusIt--;
  if ( minusIt != T.end() )
  {
    lower = *minusIt;
    return true;
  }
  else
    return false;
}


//-----------------------------------------------------------------------------
// Обработать точку события.
// ---
void ProcessPoint( std::multiset<PointEvent, SortByX>& Q, std::set<LineData*, KeySort>& T, PointEvent point,
                   std::vector<PointEvent>& intersectionPoints, std::vector<Point>& result, 
                   std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>>& params )
{
  // Если точка события - левый конец отрезка - добавляем отрезок в множество отрезков, ищем верхнего и нижнего соседа этого отрезка, проверяем на пересечение.
  if ( point.type == typeEvent::Left )
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
        PointEvent event( newPoint, point.s1, typeEvent::Intersection );
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
        PointEvent event( newPoint, upper, typeEvent::Intersection );
        event.s2 = point.s1;
        intersectionPoints.push_back( event );
      }
    }
  }
  else if ( point.type == typeEvent::Right )
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
          PointEvent event( newPoint, upper, typeEvent::Intersection );
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
    if ( FindNeighborUpper( upper, T, itS1 ) && upper != s2 )
    {
      Point newPoint;
      if ( upper->polyline != s1->polyline && ( IntersectLines( upper->line, point.s2->line, newPoint ) ||
                                                 fabs(Distance(upper->line, s2->line, newPoint) < 2. * CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        PointEvent event( newPoint, upper, typeEvent::Intersection );
        event.s2 = upper;
        intersectionPoints.push_back( event );
      }
    }
    if ( FindNeighborsLower(lower, T, itS2) && lower != s1 )
    {
      Point newPoint;
      if ( lower->polyline != s2->polyline && ( IntersectLines( lower->line, point.s1->line, newPoint ) ||
                                                 fabs(Distance(lower->line, s1->line, newPoint) < 2. *CommonConstantsMath::PRECISION_POLYLINE)) )
      {
        PointEvent event( newPoint, lower, typeEvent::Intersection );
        event.s2 = point.s1;
        intersectionPoints.push_back( event );
      }
    }
    if ( itS1 != T.end() && itS2 != T.end() && itS1 != itS2 )
    {
      s1 = *itS1;
      T.erase( itS1 );
      s2 = *itS2;
      T.erase( itS2 );
      double key = s1->key;
      s1->key = s2->key;
      s2->key = key;
      T.insert( s1 );
      T.insert( s2 );
    }

  }
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
    result.push_back( intersectionPoints[i].point );
    if ( intersectionPoints[i].s1->numParam < intersectionPoints[i].s2->numParam )
      params.push_back( std::make_pair( std::make_pair( intersectionPoints[i].s1->leftParam, intersectionPoints[i].s2->leftParam ),
                        std::make_pair(intersectionPoints[i].s1->numParam, intersectionPoints[i].s2->numParam)) );
    else
      params.push_back( std::make_pair(std::make_pair( intersectionPoints[i].s2->leftParam, intersectionPoints[i].s1->leftParam ), std::make_pair( intersectionPoints[i].s2->numParam, intersectionPoints[i].s1->numParam)) );
    if ( !FindAbscissa( intersectionPoints[i], Q ) )
    {
      Q.insert( intersectionPoints[i] );
    }
  }
  intersectionPoints.clear();
}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезков. !!! СЕЙЧАС НЕ РАБОТАЕТ, ПОПРАВИТЬ ДЛЯ ТЕСТОВ
// ---
std::vector<Point> SegmentsIntersections( std::vector<Line> segments, std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>>& params )
{
  std::vector<Point> result;
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, SortByX> Q;
  std::set<LineData*, KeySort> T;

  for ( size_t i = 0; i < segments.size(); ++i )
  {

    //  LineData currentLine(Line(segments[i - 1], segments[i]), polyline1);
    //  Q.insert(PointEvent(polylinePoints1[i - 1], currentLine, typeEvent::Left));
    //  Q.insert(PointEvent(polylinePoints1[i], currentLine, typeEvent::Right));
  }

  while ( !Q.empty() )
  {
    PointEvent currentPoint = *Q.begin();
    Q.erase( Q.begin() );
    ProcessPoint( Q, T, currentPoint, intersectPoints, result, params );
  }
  return result;
}


//-----------------------------------------------------------------------------
//  Добавить в множество точек событий опорные точки полилинии.
// ---
void CollectEventPoints( std::vector<LineData>& lines, const Math::GeomPolyline* polyline,
                         std::multiset<PointEvent, SortByX>& Q, size_t numPoliline )
{
  std::vector<Point> polylinePoints = polyline->GetReferensedPoints();
  std::vector<double> refParams = polyline->GetReferensedParams();
  size_t start = lines.size();
  for ( size_t i = 0; i < polylinePoints.size() - 1; ++i )
  {
    if ( polylinePoints[i].GetX() < polylinePoints[i + 1].GetX() )
    {
      lines.push_back( LineData( Line( polylinePoints[i], polylinePoints[i + 1] ), polyline, refParams[i], numPoliline, 0.) );
    }
    else
    {
      lines.push_back( LineData( Line( polylinePoints[i + 1], polylinePoints[i] ), polyline, refParams[i], numPoliline, 0.) );
    }
  }
  for ( size_t i = start; i < lines.size(); ++i )
  {
    // Искусственное масштабирование для правильной обработки пересечений на концах сегмента.
    Point newStart = lines[i].line.GetStartPoint() + ( lines[i].line.GetStartPoint() - lines[i].line.GetEndPoint() ) * CommonConstantsMath::SCALING;
    Point newEnd = lines[i].line.GetEndPoint() + ( lines[i].line.GetEndPoint() - lines[i].line.GetStartPoint() ) * CommonConstantsMath::SCALING;
    Q.insert( PointEvent( newStart, &lines[i], typeEvent::Left ) );
    Q.insert( PointEvent( newEnd + Point(CommonConstantsMath::NULL_TOL, CommonConstantsMath::NULL_TOL), &lines[i], typeEvent::Right ) );
  }

}

////-----------------------------------------------------------------------------
////  Найти пересечение отрезков полилилинй. В параметр params записываются параметры исходных кривых, соответсвующих пересекающимся отрезкам.
//// ---
//std::vector<Point> SegmentsIntersections( const Math::GeomPolyline* polyline1,
//                                          const Math::GeomPolyline* polyline2, std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>>& params )
//{
//  std::vector<Point> result;
//  std::vector<PointEvent> intersectPoints;
//
//  std::multiset<PointEvent, SortByX> Q;
//  std::set<LineData*, KeySort> T;
//  std::vector<LineData> lines;
//  lines.reserve( polyline1->GetReferensedPoints().size() + polyline2->GetReferensedPoints().size() );
//  CollectEventPoints( lines, polyline1, Q, 1 );
//  CollectEventPoints( lines, polyline2, Q, 2 );
//
//  while ( !Q.empty() )
//  {
//    PointEvent currentPoint = *Q.begin();
//
//    Q.erase( Q.begin() );
//    ProcessPoint( Q, T, currentPoint, intersectPoints, result, params );
//  }
//  // убираем совпадающие точки, если они есть
//
//  std::set<Point> s( result.begin(), result.end() );
//  result.assign( s.begin(), s.end() );
//
//  std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>> s1( params.begin(), params.end() );
//  params.assign( s1.begin(), s1.end() );
//  return result;
//}

//-----------------------------------------------------------------------------
//  Найти пересечение отрезков полилилинй. В параметр params записываются параметры исходных кривых, соответсвующих пересекающимся отрезкам.
// ---
std::vector<Point> SegmentsIntersections( std::vector< Math::GeomPolyline*> polyline, 
                                          std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>>& params )
{
  std::vector<Point> result;
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, SortByX> Q;
  std::set<LineData*, KeySort> T;
  std::vector<LineData> lines;
  size_t size = 0;
  for ( size_t i = 0; i < polyline.size(); i++ )
    size += polyline[i]->GetReferensedPoints().size();
  lines.reserve( size );
  for ( size_t i = 0; i < polyline.size(); i++ )
  {
    CollectEventPoints( lines, polyline[i], Q, i );
  }

  while ( !Q.empty() )
  {
    PointEvent currentPoint = *Q.begin();

    Q.erase( Q.begin() );
    ProcessPoint( Q, T, currentPoint, intersectPoints, result, params );
  }
  // убираем совпадающие точки, если они есть

  std::set<Point> s( result.begin(), result.end() );
  result.assign( s.begin(), s.end() );

  std::vector<std::pair<std::pair<double, double>, std::pair<size_t, size_t>>> s1( params.begin(), params.end() );
  params.assign( s1.begin(), s1.end() );
  return result;
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух полилиний. Это тривиальный случай: перебираем попарно все полилинии, сравниваем отреки.
// Необходим для подтверждения корректности быстрого алгоритма.
// ---
std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2, std::vector<std::pair<double, double>>& resultParams )
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
      if ( IntersectLines( Line(refPointsSecound[j - 1], refPointsSecound[j]), firstLine, point) )
      {
        auto result = std::find( std::begin(intersectPoints), std::end(intersectPoints), point );
        if ( result == std::end(intersectPoints) )
        {
          intersectPoints.push_back( point );
          resultParams.push_back( std::make_pair(refParams1[i], refParams2[j]) );
        }
      }
    }
  }
  std::set<Point> s( intersectPoints.begin(), intersectPoints.end() );
  intersectPoints.assign( s.begin(), s.end() );
  return intersectPoints;
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух полилиний. Это тривиальный случай: перебираем попарно все полилинии, сравниваем отреки.
// Необходим для подтверждения корректности быстрого алгоритма.
// ---
std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2 )
{
  std::vector<std::pair<double, double>> resultParams;
  return  IntersectPolylinePolyline( curve1, curve2, resultParams );
}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезка и полилинии.
// ---
std::vector<Point> IntersectLinePolyline( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезка и эллипса.
// ---
std::vector<Point> IntersectLineEllipse( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезка и окружности.
// ---
std::vector<Point> IntersectLineCircle( const Curve& line, const Curve& circle )
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
  double b = 2. * ( dp.GetX() * (p1.GetX() - sc.GetX()) + dp.GetY() * (p1.GetY() - sc.GetY()) );
  double c = sc * sc;
  c += p1 * p1;
  c -= 2. * ( sc * p1 );
  c -= r * r;
  double bb4ac = b * b - 4 * a * c;
  if ( fabs(a) < CommonConstantsMath::NULL_TOL || bb4ac < 0 )
  {
    return result;
  }

  const double mu1 = ( -b + sqrt(bb4ac) ) / ( 2. * a );
  const double mu2 = ( -b - sqrt(bb4ac) ) / ( 2. * a );
  if ( mu1 == mu2 )
  {
    result.push_back( p1 + (p2 - p1) * mu1 );
    return result;
  }
  else
  {
    Point point1 = p1 + ( p2 - p1 ) * mu1;
    if ( IsYinSegment( *currentLine, point1.GetY() ) && IsXinSegment(*currentLine, point1.GetX()) )
      result.push_back( point1 );
    Point point2 = p1 + ( p2 - p1 ) * mu2;

    if ( IsYinSegment(*currentLine, point2.GetY()) && IsXinSegment(*currentLine, point2.GetX()) )
      result.push_back( point2 );
    return result;
  }
}


//-----------------------------------------------------------------------------
//  Запустить метод Ньютона для поиска локального минимума квадарта расстояния между кривыми с начальной точкой в startValue.
// ---
Point NewtonMethod( const Curve& curve1, const Curve& curve2, Point startValue )
{
  size_t numiteration = CommonConstantsMath::NUMBERNEWTONMETHOD;
  Point currentPoint = startValue;
  Point newPoint;
  for ( size_t i = 0; i < numiteration; i++ )
  {
    newPoint = NewtonMethodIteration( curve1, curve2, currentPoint );
    if ( fabs( Distance(newPoint, currentPoint) ) < CommonConstantsMath::ACCURANCYMETHODNEWTON )
      break;
    currentPoint = newPoint;
  }
  return newPoint;
}


//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых, используя тривиальный метод пересечения отрезков.
// ---
std::vector<Point> IntersectGeneralCaseSimple( const Curve& curve1, const Curve& curve2 )
{
  GeomPolyline polylinePoints1;
  curve1.GetAsPolyLine( polylinePoints1 );

  GeomPolyline polylinePoints2;
  curve2.GetAsPolyLine( polylinePoints2 );
  std::vector<std::pair<double, double>> params;

  std::vector<Point> startPoints = IntersectPolylinePolyline( polylinePoints1, polylinePoints2, params );
  std::vector<Point> intersectPoints;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    Point intersectPoint = NewtonMethod( curve1, curve2, Point(params[i].first, params[i].second) );
    if ( Distance(curve1.GetPoint( intersectPoint.GetX()), curve2.GetPoint( intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( intersectPoint );
  }
  return intersectPoints;
}

//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых, используя тривиальный метод пересечения отрезков.
// ---
std::vector<Point> IntersectGeneralCaseSimple( Curve& curve1, const Curve& curve2 )
{
  GeomPolyline polylinePoints1;
  curve1.GetAsPolyLine( polylinePoints1 );

  GeomPolyline polylinePoints2;
  curve2.GetAsPolyLine( polylinePoints2 );
  std::vector<std::pair<double, double>> params;

  std::vector<Point> startPoints = IntersectPolylinePolyline( polylinePoints1, polylinePoints2, params );
  std::vector<Point> intersectPoints;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    Point intersectPoint = NewtonMethod( curve1, curve2, Point(params[i].first, params[i].second) );
    if ( Distance( curve1.GetPoint(intersectPoint.GetX()), curve2.GetPoint( intersectPoint.GetY()) ) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( intersectPoint );
  }
  return intersectPoints;
}


//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых, используя алгоритм перечения отрзков из Preparata,Sheimos.
// ---
CurveNumIntersection IntersectGeneralCase( const std::vector<Curve*> curves )
{
  std::vector<GeomPolyline*> polylinePoints;
  polylinePoints.resize( curves.size() );
  for ( size_t i = 0; i < curves.size(); i++ )
  {
    polylinePoints[i] = new GeomPolyline();
    curves[i]->GetAsPolyLine( *polylinePoints[i] );
  }

  CurveNumIntersection params;
  SegmentsIntersections( polylinePoints, params );
  CurveNumIntersection intersectPoints;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    Point intersectPoint = NewtonMethod( *curves[ params[i].second.first], *curves[params[i].second.second], Point(params[i].first.first, params[i].first.second) );
    if ( Distance(curves[params[i].second.first]->GetPoint(intersectPoint.GetX()), curves[params[i].second.second]->GetPoint( intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( std::make_pair(std::make_pair( intersectPoint.GetX(), intersectPoint.GetY()), params[i].second) );
  }
  return intersectPoints;

}


}