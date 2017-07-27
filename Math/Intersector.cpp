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
std::vector<std::vector<double>> CountingHessian(const Curve& curve1, const Curve& curve2, double t1, double t2)
{
  const double x1 = curve1.GetPoint(t1).GetX();
  const double y1 = curve1.GetPoint(t1).GetY();
  const double x1d = curve1.GetDerivative(t1).GetX();
  const double y1d = curve1.GetDerivative(t1).GetY();
  const double x1dd = curve1.Get2Derivative(t1).GetX();
  const double y1dd = curve1.Get2Derivative(t1).GetY();

  const double x2 = curve2.GetPoint(t2).GetX();
  const double y2 = curve2.GetPoint(t2).GetY();
  const double x2d = curve2.GetDerivative(t2).GetX();
  const double y2d = curve2.GetDerivative(t2).GetY();
  const double x2dd = curve2.Get2Derivative(t2).GetX();
  const double y2dd = curve2.Get2Derivative(t2).GetY();

  const double F11 = x1dd * 2. * (x1 - x2) + 2. * x1d * x1d + y1dd * 2. * (y1 - y2) + 2. * y1d * y1d;
  const double F12 = -2. * x2d * x1d - 2. * y2d * y1d;
  const double F21 = -2. *x1d * x2d - 2. * y1d * y2d;
  const double F22 = -2. * (x1 - x2) * x2dd + 2.* x2d * x2d - 2. * (y1 - y2) * y2dd + 2. * y2d * y2d;

  std::vector<std::vector <double>> hessian;
  hessian.resize(2);
  hessian[0].resize(2);
  hessian[1].resize(2);
  hessian[0][0] = F11;
  hessian[0][1] = F12;
  hessian[1][0] = F21;
  hessian[1][1] = F22;
  return hessian;
}


//-----------------------------------------------------------------------------
//  Вернуть обратную матрицу. Предполагается, что матрица невырождена.
// ---
std::vector<std::vector<double>> InverseMatrix( const std::vector<std::vector<double>>& matrix )
{
  std::vector<std::vector<double>> result( 2, std::vector<double>(2) );
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
  Point point1 = curve1.GetPoint( point.GetX() );
  Vector grad1 = curve1.GetDerivative( point.GetX() );

  Point point2 = curve2.GetPoint(point.GetY());
  Vector grad2 = curve2.GetDerivative(point.GetY());
  double aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  double aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
  return Vector( aResultT1, aResultT2 );
}


//-----------------------------------------------------------------------------
//  Провести одну итерацию метода Ньютона.
// ---
Point NewtonMethodIteration( const Curve& curve1, const Curve& curve2, Point start )
{
  auto invHessian = InverseMatrix(CountingHessian(curve1, curve2, start.GetX(), start.GetY()));
  const Vector grad = Gradient(curve1, curve2, start);
  const Point step(invHessian[0][0] * -grad.GetX() + invHessian[0][1] * -grad.GetY(), invHessian[1][0] *
    -grad.GetX() + invHessian[1][1] * -grad.GetY());
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
  if ( fabs(a * testPoint.GetX() + b * testPoint.GetY() + c) > CommonConstantsMath::NULL_TOL )
    return false;

  return IsYinSegment( line, testPoint.GetY()) && IsYinSegment(line, testPoint.GetX() );
}
}

//-----------------------------------------------------------------------------
//  Найти пересечение полилинии и окружности.
// ---
std::vector<Point> IntersectPolylineCircle( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение полилинии и эллипса.
// ---
std::vector<Point> IntersectPolylineEllipse( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух отрезков.
// ---
bool IntersectLines( const Curve& curve1, const Curve& curve2, Point& thePoint )
{
  std::vector<Point> IntersectPoints;
  const Math::Line* lineCurveFirst = dynamic_cast<const Math::Line*>( &curve1 );
  const Math::Line* lineCurveSecound = dynamic_cast<const Math::Line*>( &curve2 );

  const Vector dir1 = lineCurveFirst->GetEndPoint() - lineCurveFirst->GetStartPoint();
  const Vector dir2 = lineCurveSecound->GetEndPoint() - lineCurveSecound->GetStartPoint();
  const Point start1 = lineCurveFirst->GetStartPoint();
  const Point start2 = lineCurveSecound->GetStartPoint();
  const Point end1 = lineCurveFirst->GetEndPoint();
  const Point end2 = lineCurveSecound->GetEndPoint();

  if ( !dir1.IsCollinear(dir2) )
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
    bool isInOneLine = ( Vector(start1 - start2).IsCollinear(Vector(start2 - end1))) && (Vector(start1 - start2).IsCollinear(Vector(start2 - end2)) );
    if ( isInOneLine )
    {
      if ( IsEqual(start1, end2) )
      {
        thePoint = end2;
        return true;
      }
      if ( IsEqual(start2, end1) )
      {
        thePoint = end1;
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
enum typeEvent {
  Intersection, // Точка пересечения отрезков.
  Left,         // Левый конец отрезка.
  Right,        // Правый конец отрезка.
  NonValid
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
  size_t                    indexNumber; // Номер линии в списке линий.

  LineData() = default;
  LineData      ( Line theLine, const GeomPolyline* thePolyline, double theLeftParam, size_t thenumParam, size_t theIndexNumber ) :
      line        ( theLine ),
      polyline    ( thePolyline ),
      leftParam   ( theLeftParam),
      numParam    ( thenumParam ),
      indexNumber ( theIndexNumber )
  {}

  bool operator==( const LineData& obj ) const
  {
    return obj.line == line && obj.polyline == polyline;
  }
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
  LineData  s1;   // Отрезок, соответствующий точке события.
  LineData  s2;
  size_t    indexNumber; // Номер отрезка, которому принадлежит эта точка.
  PointEvent () : point( 0., 0. ) {}
  PointEvent ( Point thePoint, LineData theS1, typeEvent theType, size_t theIndexNumber ) :
    point       ( thePoint ),
    type        ( theType ),
    s1          ( theS1 ),
    indexNumber ( theIndexNumber )
   {}
  PointEvent& operator = ( const PointEvent &obj )
  {
    type = obj.type;
    point = obj.point;
    s1 = obj.s1;
    s2 = obj.s2;
  }
  bool operator == ( PointEvent rhs ) const
  {
    return ( IsEqual(point, rhs.point) );
  }
};


struct PointHash {
  size_t operator() ( const PointEvent &point ) const
  {
    return (size_t)( point.point.GetX() * 100 + point.point.GetY() );
  }
};


//inline bool operator == ( const PointEvent lhs, const PointEvent rhs ) 
//{
//  return ( IsEqual(lhs.point, rhs.point) );
//}
//-----------------------------------------------------------------------------
//  Функтор лексиграфического упорядочивания точек по возрастанию.
// ---
struct SortByX
{
  inline bool operator () ( const PointEvent &lhs, const PointEvent& rhs ) const
  {
    if ( lhs.point.GetX() > rhs.point.GetX() )
      return false;
    if ( lhs.point.GetX() < rhs.point.GetX() )
      return true;
    return ( lhs.point.GetY() < rhs.point.GetY() );
  }
};



//-----------------------------------------------------------------------------
// Получить по координате х, принадлежащей отрезку, координату y.
// ---
double GetYFromX(const Line& line, double x)
{
  // !!! тут деление на ноль для вертикального отрезка
  double x1 = line.GetStartPoint().GetX();
  double y1 = line.GetStartPoint().GetY();
  double x2 = line.GetEndPoint().GetX();
  double y2 = line.GetEndPoint().GetY();
  double ch = -(x1 * y2 - x2 * y1) - x * (y1 - y2);
  return ch / (x2 - x1);
}

struct SortByY
{
  inline bool operator () ( const LineData&lhs, const LineData& rhs ) const
  {
    Point startPointLhs = lhs.line.GetStartPoint();
    Point startPointRhs = rhs.line.GetStartPoint();

    Point endPointLhs = lhs.line.GetEndPoint();
    Point endPointRhs = rhs.line.GetEndPoint();
    if ( IsEqual(startPointLhs, startPointRhs) && IsEqual(endPointLhs, endPointRhs) )
      return false;
    if ( IsEqual(startPointLhs, startPointRhs) )
    {
      double endX = 0.0;
      if ( endPointLhs.GetX() < endPointRhs.GetX() )
      {
        endX = endPointLhs.GetX();
        double y = GetYFromX( rhs.line, endX );
        return ( y < endPointLhs.GetY() );
      }
      else
      {
        endX = endPointRhs.GetX();
        double y = GetYFromX( lhs.line, endX );
        return (y > endPointRhs.GetY());
      }
    }
    double startX = 0.0;
    if (IsEqual(startPointLhs, endPointRhs))
      return false;
    if (IsEqual(startPointRhs, endPointLhs))
      return true;
    if ( startPointLhs.GetX() > startPointRhs.GetX() )
    {
      startX = startPointLhs.GetX();
      double y = GetYFromX( rhs.line, startX );
      return ( y < startPointLhs.GetY() );
    }
    else
    {
      startX = startPointRhs.GetX();
      double y = GetYFromX( lhs.line, startX);
      return ( y > startPointRhs.GetY() );
    }
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
// Найти верхнего и нижнего соседа для текущей точки события.
// ---
void FindNeighbors( PointEvent& upper, PointEvent& lower, const std::set<PointEvent>& T,const PointEvent& point )
{/*
  upper.point = Point( point.point.GetX(), std::numeric_limits<double>::lowest() );
  lower.point = Point( point.point.GetX(), std::numeric_limits<double>::max() );

  for ( auto it = T.begin(); it != T.end(); it++ )
  {
    if ( it->point.GetY() < upper.point.GetY() && it->point.GetY() > point.point.GetY() )
      upper = *it;
    if ( it->point.GetY() > lower.point.GetY() && it->point.GetY() < point.point.GetY() )
      lower = *it;
  }*/
}



//-----------------------------------------------------------------------------
// Получить для текущей точки события верхнего соседа.
// ---
bool FindNeighborUpper( std::vector<LineData>& upper, const std::set<LineData, SortByY>& T, const LineData& line )
{
  auto it = T.find( line );
  if ( it == T.end() )
    for ( auto iter = T.begin(); iter != T.end();iter++ )
      if ( iter->indexNumber == line.indexNumber )
      {
        it = iter; break;
      }
  auto minusIt = it; minusIt++;
  if ( minusIt != T.end() )
    upper.push_back( *(minusIt) );
  return !upper.empty();
}


//-----------------------------------------------------------------------------
// Получить для текущей точки события нижнего соседа.
// ---
bool FindNeighborsLower( std::vector<LineData>& lower, std::set<LineData, SortByY>& T, const LineData& line )
{
  auto it = T.find( line );
  if ( it == T.end() )
    for ( auto iter = T.begin(); iter != T.end(); iter++ )
      if ( iter->indexNumber == line.indexNumber )
      {
        it = iter; break;
      }
  auto plusIt = it; plusIt--;
  if ( plusIt != T.end() )
    lower.push_back( *(plusIt) );
  return !lower.empty();
}



void DeleteCurrentLine( std::set<LineData, SortByY>& T, const PointEvent& point )
{
  for ( auto iter = T.begin(); iter != T.end(); )
  {
    if ( iter->indexNumber == point.indexNumber )
    {
      T.erase( iter++ );
      break;
    }
    else
    {
      ++iter;
    }
  }
}

//-----------------------------------------------------------------------------
// Обработать точку события.
// ---
void ProcessPoint( std::multiset<PointEvent, SortByX>& Q, std::set<LineData, SortByY>& T, PointEvent point,
  std::vector<PointEvent>& intersectionPoints, std::vector<Point>& result, std::vector<std::pair<double,double>>& params )
{
  if ( point.type == typeEvent::Left )
  {
  auto it = T.insert( point.s1 );
    std::vector<LineData> lower;

    if ( FindNeighborsLower(lower, T, point.s1) )
    {
      Point newPoint;
      for ( size_t i = 0; i < lower.size(); i++ )
      {
        if ( lower[i].polyline != point.s1.polyline && (IntersectLines(lower[i].line, point.s1.line, newPoint)
          /* fabs(Distance( lower.line, point.s1.line, newPoint) < CommonConstantsMath::NULL_TOL))*/))
        {
          PointEvent event( newPoint, point.s1, typeEvent::Intersection, 0 );
          event.s2 = lower[i];
          intersectionPoints.push_back(event);
        }
      }
    }
    std::vector<LineData> upper;
    if ( FindNeighborUpper(upper, T, point.s1) )
    {
      for ( size_t i = 0; i < upper.size(); i++ )
      {
      Point newPoint;
      if (upper[i].polyline != point.s1.polyline && (IntersectLines(upper[i].line, point.s1.line, newPoint)
        /*(fabs (Distance(upper.line, point.s1.line, newPoint))  < CommonConstantsMath::NULL_TOL )*/))
      {
        PointEvent event( newPoint, upper[i], typeEvent::Intersection, 0 );
        event.s2 = point.s1;
        intersectionPoints.push_back(event);
      }
    }
  }
  }
  else if ( point.type == typeEvent::Right )
  {
    std::vector<LineData> lower;
    std::vector<LineData> upper;
    if ( FindNeighborsLower(lower, T, point.s1) && FindNeighborUpper(upper, T, point.s1) )
    {
      Point newPoint;
      for ( size_t i = 0; i < lower.size(); i++ )
        for ( size_t j = 0; j < upper.size(); j++ )
        {
          if ( lower[i].polyline != upper[j].polyline && (IntersectLines(upper[j].line, lower[i].line, newPoint )
            /*fabs(Distance( upper.line,lower.line, newPoint) < CommonConstantsMath::NULL_TOL)*/))
          {
            if ( newPoint.GetX() > point.point.GetX() )
            {
              PointEvent event( newPoint, upper[j], typeEvent::Intersection, 0 );
              event.s2 = lower[i];
              intersectionPoints.push_back( event );
            }
          }
    }
    }
    DeleteCurrentLine( T, point );
  }
  else
  {
    LineData s1 = point.s1;
    LineData s2 = point.s2;
    std::vector<LineData> lower;
    std::vector<LineData> upper;
    if ( FindNeighborUpper(lower, T, s1) )
    {
      for ( size_t i = 0; i < lower.size(); i++ )
      {
        Point newPoint;
        if ( lower[i].polyline != s1.polyline && (IntersectLines(lower[i].line, point.s1.line, newPoint)
          /* fabs(Distance(lower.line, s1.line, newPoint) < CommonConstantsMath::NULL_TOL))*/) )
        {
          PointEvent event( newPoint, lower[i], typeEvent::Intersection, 0 );
          event.s2 = lower[i];
          intersectionPoints.push_back(event);
        }
      }
    }
    if ( FindNeighborsLower(upper, T, s2) )
    {
      for ( size_t i = 0; i < upper.size(); i++ )
      {
        Point newPoint;
        if ( upper[i].polyline != s2.polyline && (IntersectLines(upper[i].line, point.s2.line, newPoint)
          /* fabs(Distance(upper.line, s2.line, newPoint) < CommonConstantsMath::NULL_TOL))*/) )
        {
          PointEvent event( newPoint, upper[i], typeEvent::Intersection, 0 );
          event.s2 = point.s1;
          intersectionPoints.push_back( event );
        }
      }
    }
   
    auto it = T.find( s1 );
   
    auto it2 = T.find(s2);
    if ( it != T.end() && it2 != T.end() )
    {
      s1 = *it;
      T.erase(it);
      it2 = T.find(s2);
      s2 = *it2;
      T.erase(it2);
      Point newPoint;
      IntersectLines(s1.line, s2.line, newPoint);
      s1.line = Line(newPoint, s1.line.GetEndPoint());
      s2.line = Line(newPoint, s2.line.GetEndPoint());
      T.insert(s1);
      T.insert(s2);
    }

  }
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
    result.push_back( intersectionPoints[i].point );
    if ( intersectionPoints[i].s1.numParam < intersectionPoints[i].s2.numParam )
      params.push_back( std::make_pair(intersectionPoints[i].s1.leftParam, intersectionPoints[i].s2.leftParam) );
    else 
      params.push_back( std::make_pair(intersectionPoints[i].s2.leftParam, intersectionPoints[i].s1.leftParam) );
    if ( !FindAbscissa(intersectionPoints[i], Q) )
    {  
      Q.insert( intersectionPoints[i] );
    }
  }
  intersectionPoints.clear();
}


//-----------------------------------------------------------------------------
//  Найти пересечение отрезков. !!! СЕЙЧАС НЕ РАБОТАЕТ, ПОПРАВИТЬ ДЛЯ ТЕСТОВ
// ---
std::vector<Point> SegmentsIntersections( std::vector<Line> segments , std::vector<std::pair<double, double>>& params )
{
  std::vector<Point> result;
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, SortByX> Q;
  std::set<LineData, SortByY> T;

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
void CollectEventPoints( const Math::GeomPolyline* polyline, std::multiset<PointEvent, SortByX>& Q, size_t numPoliline, size_t numIndex )
{
  std::vector<Point> polylinePoints = polyline->GetReferensedPoints();
  std::vector<double> refParams = polyline->GetReferensedParams();
  for ( size_t i = 0; i < polylinePoints.size() - 1; ++i )
  {
    if ( polylinePoints[i].GetX() < polylinePoints[i + 1].GetX() )
    {
      LineData currentLine( Line(polylinePoints[i], polylinePoints[i + 1]), polyline, refParams[i], numPoliline, i + numIndex );
      Q.insert( PointEvent(polylinePoints[i], currentLine, typeEvent::Left, i + numIndex) );
      Q.insert( PointEvent(polylinePoints[i + 1], currentLine, typeEvent::Right, i + numIndex) );
    }
    else
    {
      LineData currentLine( Line(polylinePoints[i + 1], polylinePoints[i]), polyline, refParams[i], numPoliline, i + numIndex );
      Q.insert( PointEvent(polylinePoints[i], currentLine, typeEvent::Right, i + numIndex) );
      Q.insert( PointEvent(polylinePoints[i + 1], currentLine, typeEvent::Left, i + numIndex) );
    }
  }

}

//-----------------------------------------------------------------------------
//  Найти пересечение отрезков полилилинй. В параметр params записываются параметры исходных кривых, соответсвующих пересекающимся отрезкам.
// ---
std::vector<Point> SegmentsIntersections( const Math::GeomPolyline* polyline1,
  const Math::GeomPolyline* polyline2, std::vector<std::pair<double,double>>& params )
{
  std::vector<Point> result;
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, SortByX> Q;
  std::set<LineData, SortByY> T;

  CollectEventPoints( polyline1, Q, 1, 0 );
  CollectEventPoints( polyline2, Q, 2, polyline1->GetReferensedPoints().size() );

  while ( !Q.empty() )
  {
    PointEvent currentPoint = *Q.begin();

    Q.erase( Q.begin() );
    ProcessPoint( Q, T, currentPoint, intersectPoints, result, params );
  }
  // убираем совпадающие точки, если они есть

  std::set<Point> s( result.begin(), result.end() );
  result.assign( s.begin(), s.end() );

  std::set<std::pair<double,double>> s1( params.begin(), params.end());
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
  const Math::GeomPolyline* lineCurveFirst =  dynamic_cast<const Math::GeomPolyline*>( &curve1 );
  std::vector<Point> refPointsFirst = lineCurveFirst->GetReferensedPoints();
  const Math::GeomPolyline* lineCurveSecound =  dynamic_cast<const Math::GeomPolyline*>( &curve2 );

  std::vector<double> refParams1 = lineCurveFirst->GetReferensedParams();
  std::vector<double> refParams2 = lineCurveSecound->GetReferensedParams();

  std::vector<Point> refPointsSecound = lineCurveSecound->GetReferensedPoints();
  for ( size_t i = 1; i < refPointsFirst.size(); ++i )
  {
    Line firstLine( refPointsFirst[i - 1], refPointsFirst[i] );
    for ( size_t j = 1; j < refPointsSecound.size(); ++j )
    {
      Point point;
      if ( IntersectLines(Line(refPointsSecound[j - 1], refPointsSecound[j]), firstLine, point) )
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
std::vector<Point> IntersectPolylinePolyline(const Curve& curve1, const Curve& curve2)
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
  const Math::Line* currentLine = dynamic_cast<const Math::Line*>( &line );
  const Math::Ellipse* currentCircle = dynamic_cast<const Math::Ellipse*>( &circle );
  Point p1 = currentLine->GetStartPoint();
  Point p2 = currentLine->GetEndPoint();
  Point sc = currentCircle->GetCenter();
  double r = currentCircle->GetMajorRadius();

  Vector dp = p2 - p1;

  double a = dp * dp;
  double b = 2. * ( dp.GetX() * (p1.GetX()- sc.GetX()) + dp.GetY() * (p1.GetY() - sc.GetY()) );
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
    result.push_back( p1 + ( p2 - p1 ) * mu1 );
    return result;
  } 
  else
  {
    Point point1 = p1 + ( p2 - p1 ) * mu1;
    if ( IsYinSegment(*currentLine, point1.GetY()) && IsXinSegment(*currentLine, point1.GetX()) )
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
  size_t numiteration = 10;
  Point currentPoint = startValue;
  Point newPoint;
  for ( size_t i = 0; i < numiteration; i++ ) 
  {
    newPoint = NewtonMethodIteration( curve1, curve2, currentPoint );
    if ( fabs(Distance(newPoint, currentPoint)) < CommonConstantsMath::NULL_TOL )
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
    if ( Distance( curve1.GetPoint(intersectPoint.GetX()), curve2.GetPoint(intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL )
          intersectPoints.push_back( intersectPoint );
	}
  return intersectPoints;
}


//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых, используя алгоритм перечения отрзков из Preparata,Sheimos.
// ---
std::vector<Point> IntersectGeneralCase( const Curve& curve1, const Curve& curve2 )
{
  GeomPolyline polylinePoints1;
  std::vector<double> refcurve1;
  curve1.GetAsPolyLine( polylinePoints1 );

  GeomPolyline polylinePoints2;
  std::vector<double> refcurve2;
  curve2.GetAsPolyLine( polylinePoints2 );

  std::vector<std::pair<double, double>> params;
  SegmentsIntersections( &polylinePoints1, &polylinePoints2, params );
  std::vector<Point> intersectPoints;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    Point intersectPoint = NewtonMethod( curve1, curve2, Point(params[i].first, params[i].second) );
    if ( Distance(curve1.GetPoint(intersectPoint.GetX()), curve2.GetPoint(intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL )
      intersectPoints.push_back( intersectPoint );
  }
  return intersectPoints;

}


//-----------------------------------------------------------------------------
//  В зависимости от типа кривой запустить нужный алгоритм для поиска пересечения.
// ---
std::vector<Point> IntersectCurves( const Curve& curve1, const Curve& curve2 )
{
  std::vector<Point> intersectCurves;
  Curve::CurveType typeCurve1 = curve1.GetType();
  Curve::CurveType typeCurve2 = curve2.GetType();

  switch ( typeCurve1 )
  {
    case Curve::LineType :
      switch ( typeCurve2 )
      {
      case Curve::LineType: {
        Point point;
        if ( IntersectLines(curve1, curve2, point) )
          intersectCurves.push_back( point );
        return intersectCurves;
      }
      case Curve::PolylineType :
        return IntersectLinePolyline( curve1, curve2 );
      case Curve::CircleType :
        return IntersectLineCircle( curve1, curve2 );
      case Curve::EllipseType :
        return IntersectLineEllipse( curve1, curve2);
      case Curve::NurbsType :
        return IntersectGeneralCase( curve1, curve2 );
      }
    case Curve::PolylineType :
      switch  ( typeCurve2 )
      {
        case Curve::PolylineType :
          return IntersectPolylinePolyline( curve1, curve2);
        case Curve::CircleType :
          return IntersectPolylineCircle( curve1, curve2 );
        case Curve::EllipseType :
          return IntersectPolylineEllipse( curve1, curve2 );
        case Curve::NurbsType :
          return IntersectGeneralCase( curve1, curve2 );
      }
    case Curve::CircleType :
      return IntersectGeneralCase( curve1, curve2 );
    case Curve::EllipseType :
      return IntersectGeneralCase( curve1, curve2 );
     case Curve::NurbsType :
      return IntersectGeneralCase( curve1, curve2 );
    break;

  }
  return intersectCurves;
}


}

