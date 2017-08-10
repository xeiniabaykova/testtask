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
  const auto t1Point = curve1.GetPoint( t1 );
  const auto x1 = t1Point.GetX();
  const auto y1 = t1Point.GetY();

  const auto t1PointD = curve1.GetDerivative( t1 );
  const auto x1d = t1PointD.GetX();
  const auto y1d = t1PointD.GetY();

  const auto t1Point2D = curve1.Get2Derivative( t1 );
  const auto x1dd = t1Point2D.GetX();
  const auto y1dd = t1Point2D.GetY();

  const auto t2Point = curve2.GetPoint( t2 );
  const auto x2 = t2Point.GetX();
  const auto y2 = t2Point.GetY();

  const auto t2PointD = curve2.GetDerivative( t2 );
  const auto x2d = t2PointD.GetX();
  const auto y2d = t2PointD.GetY();

  const auto t2Point2D = curve2.Get2Derivative( t2 );
  const auto x2dd = curve2.Get2Derivative( t2 ).GetX();
  const auto y2dd = curve2.Get2Derivative( t2 ).GetY();

  Matrix22 hessian;
  hessian[0][0] = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  hessian[0][1] = -2. * x2d * x1d - 2. * y2d * y1d;
  hessian[1][0] = -2. *x1d * x2d - 2. * y1d * y2d;
  hessian[1][1] = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;

  return hessian ;
}


//-----------------------------------------------------------------------------
//  Вернуть обратную матрицу. Предполагается, что матрица невырождена.
// ---
static Matrix22& InverseMatrix( Matrix22& matrix )
{
  Matrix22 result( matrix );
  const auto determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  matrix[0][0] = result[1][1] / determinant;
  matrix[0][1] = -result[1][0] / determinant;
  matrix[1][0] = -result[0][1] / determinant;
  matrix[1][1] = result[0][0] / determinant;
  return matrix;
}


//-----------------------------------------------------------------------------
//  Вернуть градиент для квадрата расстояния между кривыми в заданной точке.
// ---
static Vector Gradient( const Curve& curve1, const Curve& curve2, const double& paramCurve1, const double& paramCurve2 )
{
  const auto point1 = curve1.GetPoint( paramCurve1 );
  const auto grad1 = curve1.GetDerivative( paramCurve1 );
  const auto point2 = curve2.GetPoint( paramCurve2 );
  const auto grad2 = curve2.GetDerivative( paramCurve2 );
  const auto aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  const auto aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
  return Vector( aResultT1, aResultT2 );
}


//-----------------------------------------------------------------------------
//  Вернуть расстояние между двумя параметрическими точками.
// ---
static double Distance( const double& paramCurve11, const double& paramCurve12, const double& paramCurve21, const double& paramCurve22 )
{
  return sqrt( (paramCurve11 - paramCurve21) * (paramCurve11 - paramCurve21) + (paramCurve12 - paramCurve22) * (paramCurve12 - paramCurve22) );
}


//-----------------------------------------------------------------------------
//  Запустить метод Ньютона для поиска локального минимума квадрата расстояния между кривыми curve1, curve2
//  с начальными значениями параметров кривых paramCurve1,paramCurve2.
// Если метод не сходится к решению, то возвращается false. Если сходится к решению, то возвращается true, а в параметрах
// paramCurve1,paramCurve2 возвращается решение.
// ---
static bool NewtonMethod( const Curve& curve1, const Curve& curve2, double& paramCurve1, double& paramCurve2 )
{
  auto currentParamCurve1 = paramCurve1;
  auto currentParamCurve2 = paramCurve2;
  Matrix22 lastNonZeroMatrix = { 1., 0., 0., 1. };
  auto newParamCurve1 = paramCurve1;
  auto newParamCurve2 = paramCurve2;
  for ( size_t i = 0; i < CommonConstantsMath::NUMBER_NEWTON_METHOD; i++ )
  {
    auto invHessian = CountingHessian( curve1, curve2, currentParamCurve1, currentParamCurve2 );
    if ( fabs(invHessian[0][0] * invHessian[1][1] - invHessian[0][1] * invHessian[1][0]) > CommonConstantsMath::NULL_TOL )
      lastNonZeroMatrix = invHessian;
    else
      invHessian = lastNonZeroMatrix;

    invHessian = InverseMatrix( invHessian );
    const auto grad = Gradient( curve1, curve2, currentParamCurve1, currentParamCurve2 );
    auto stepParam1 = invHessian[0][0] * -grad.GetX() + invHessian[0][1] * -grad.GetY();
    auto stepParam2 = invHessian[1][0] * -grad.GetX() + invHessian[1][1] * -grad.GetY();

    newParamCurve1 = currentParamCurve1 + stepParam1;
    newParamCurve2 = currentParamCurve2 + stepParam2;

    if ( fabs( Distance( newParamCurve1, newParamCurve2, currentParamCurve1, currentParamCurve2 ) ) < CommonConstantsMath::ACCURANCY_METHOD_NEWTON )
    {
      paramCurve1 = newParamCurve1;
      paramCurve2 = newParamCurve2;
      return true;
    }
    currentParamCurve1 = newParamCurve1;
    currentParamCurve2 = newParamCurve2;
  }
  return false;
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
static bool IsXinSegment( const Line& line, const double& x )
{
  return ( x - line.GetStartPoint().GetX() ) * ( line.GetEndPoint().GetX() - x ) >= 0;
}


//-----------------------------------------------------------------------------
// Проверить, содержит ли отрезок точку с заданной координатой y.
// ---
static bool IsYinSegment( const Line& line, const double& y )
{
  return ( y - line.GetStartPoint().GetY() ) * ( line.GetEndPoint().GetY() - y ) >= 0;
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух отрезков.
// ---
static bool IntersectLines( const Math::Line& lineCurveFirst, const Math::Line& lineCurveSecond, Point& intersectionPoint  )
{
  const auto dir1 = lineCurveFirst.GetDerivative( 0. );
  const auto dir2 = lineCurveSecond.GetDerivative( 0. );
  const auto start1 = lineCurveFirst.GetStartPoint();
  const auto start2 = lineCurveSecond.GetStartPoint();
  const auto rX = start1.GetX() - start2.GetX();
  const auto rY = start1.GetY() - start2.GetY();
  if ( !dir1.IsCollinear(dir2) )
  {
    const auto det = (dir2.GetX() * -dir1.GetY() + dir1.GetX() * dir2.GetY());
    const auto det1 = rY * dir2.GetX() - dir2.GetY() * rX;
    const auto det2 = -dir1.GetY() * rX + dir1.GetX() * rY;
    const auto t1Intersect = det1 / det;
    const auto t2Intersect = det2 / det;

    if ( 0. <= t1Intersect && 1. >= t1Intersect && 0. <= t2Intersect && 1. >= t2Intersect )
    {
      intersectionPoint = start1 + dir1 * t1Intersect;
      return true;
    }
  }
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
  const Line               line;      // Отрезок.
  const GeomPolyline&      polyline;  // Полилиния, которой принадлежит отрезок.
  const Curve&             curve;    // Кривая, соответствующая полилинии, которой принадлежит отрезок.
  const double             leftParam; // Параметр исходной кривой, соответсующий отрезку полилинии.
  LineData    ( const Line& theLine, const GeomPolyline& thePolyline, const double& theLeftParam, const Curve& theCurve ):
    line      ( theLine ),
    polyline  ( thePolyline ),
    leftParam ( theLeftParam ),
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
  TypeEvent type;          // Тип точки события.
  const Point     point;   // Точка события.
  const LineData  &s1;     // Отрезок, соответствующий точке события.
  const LineData  *s2;     // Если точка события является точкой пересечения, то храним второй отрезок тоже.
  PointEvent ( Point thePoint, const LineData &theS1, const TypeEvent& theType ):
    point    ( thePoint ),
    type     ( theType ),
    s1       ( theS1 ),
    s2       ( nullptr )
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
//  Функтор лексиграфического упорядочивания точек по возрастанию.
// ---
struct IsLexLessX
{
  inline bool operator()( const PointEvent &lhs, const PointEvent& rhs ) const
  {
    if ( IsSame(lhs.point, rhs.point) )
    {
      return IsLexLessFromX( lhs.s1.line.GetEndPoint(), rhs.s1.line.GetEndPoint() );
    }
    return IsLexLessFromX( lhs.point, rhs.point );
  }
};


//-----------------------------------------------------------------------------
// Получить по координате х, принадлежащей отрезку, координату y.
// ---
double GetYFromX( const Line& line, const double& x )
{
  const auto x1 = line.GetStartPoint().GetX();
  const auto y1 = line.GetStartPoint().GetY();
  const auto x2 = line.GetEndPoint().GetX();
  const auto y2 = line.GetEndPoint().GetY();
  if ( fabs(x1 - x2) < CommonConstantsMath::NULL_TOL )
    return y2;
  return y1 + (y2 - y1)*(x - x1) / (x2 - x1);
}


//-----------------------------------------------------------------------------
//  Функтор лексикографического упорядочивания отрезков в текущей координате x. Текущая координата х - координата х
//  текущей точки события.
//--
double currentX; // Текущая координата х.
struct KeySort
{
  inline bool operator () ( const LineData* lhs, const LineData* rhs ) const
  {
    const auto lhsY = GetYFromX( lhs->line, currentX );
    const auto rhsY = GetYFromX( rhs->line, currentX );
    if ( lhsY == rhsY )
    {
      if ( IsSame(lhs->line.GetStartPoint(), rhs->line.GetStartPoint()) )
        return  GetYFromX( lhs->line, lhs->line.GetEndPoint().GetX() ) < GetYFromX( rhs->line, lhs->line.GetEndPoint().GetX() );
    }
    return ( lhsY < rhsY );
  }

};


//-----------------------------------------------------------------------------
// Получить для текущей точки события верхнего соседа. Если соседа нет,возвращаем false.
// ---
static bool FindNeighborUpper( const LineData*& upper, const std::set<const LineData*, KeySort>& setProcessedSegments, std::set<const LineData*, KeySort>::iterator it )
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
static bool FindNeighborsLower( const LineData*& lower, std::set<const LineData*, KeySort>& setProcessedSegments, std::set<const LineData*, KeySort>::iterator it )
{
  if ( --it != setProcessedSegments.end() )
  {
    lower = *it;
    return true;
  }
  else
    return false;
}


Point oldPoint; // Предыдущая точка события. Необходим для того, чтобы при пересечении находить нужную координату упорядочивания "До" точки пересечения.

// Установленное упорядочивание отрезков может быть неправильным из-за накопившеся числовой ошибки.
// Преобразовать упорядочивание к паравильному относительно текущего x.
void RestoreSanity( std::set<const LineData*, KeySort>& currentSegments )
{
  std::set<const LineData*, KeySort> newSet;
  for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
    newSet.insert( *it );
  currentSegments = newSet;
}


//-----------------------------------------------------------------------------
// В случае, когда исходная кривая отрезок или полилиния, возвращаем параметр, 
//соотвествующий точке. В другом случае возвращаем ближайшую границу параметра.
// ---
double GetParamFromType( const Curve& intersectCurve, const double& leftParam, const Point& point )
{
  if ( intersectCurve.GetType() == Curve::CurveType::LineType )
    return static_cast<const Line*>( &intersectCurve )->GetTFromPoint( point );
  else if ( intersectCurve.GetType() == Curve::CurveType::PolylineType )
    return static_cast<const GeomPolyline*>( &intersectCurve )->GetTFromPoint( point );
  else return leftParam;
}


//-----------------------------------------------------------------------------
// Обработать пересечение прямых. Если прямые пересекаются и полилинии совпадают, то заносим точку пересечений в множество событий,
// но не добавляем к множеству точек пересечения. Если же прямые не пересекаются, но находятся на достаточно близком расстоянии
// друг от друга, то не добавляем эту точку в множество событий, но добавляем к множеству точек пересечения.
// ---
void IntersectLinesEvent( const LineData* line1, const LineData* line2, std::vector<PointEvent>& intersectionPoints,
                          std::multiset<PointEvent, IsLexLessX>& setEventPoints, const double& accuracyPolyliline )
{
  if ( line1 != nullptr && line2 != nullptr )
  {
    Point newPoint;
    if ( ( IntersectLines( line2->line, line1->line, newPoint ) ) )
    {
      PointEvent event( newPoint, *line2, TypeEvent::Intersection );
      event.s2 = line1;
      setEventPoints.insert( event );
      if ( &line1->polyline != &line2->polyline )
        intersectionPoints.push_back( event );
    }
    else if ( &line1->polyline != &line2->polyline &&
      ( fabs( Distance( line1->line, line2->line, newPoint ) ) < 2. * accuracyPolyliline ) )
    {
      PointEvent event( newPoint, *line2, TypeEvent::Intersection );
      event.s2 = line1;
      intersectionPoints.push_back( event );
    }
  }
}


//-----------------------------------------------------------------------------
// Обработать точку события.
// ---
static void ProcessPoint( std::multiset<PointEvent, IsLexLessX>& setEventPoints, std::set<const LineData*, KeySort>& currentSegments, const PointEvent& point,
                          std::vector<PointEvent>& intersectionPoints,
                          std::vector<CurveIntersectionData>& params, const double& accuracyPolyliline )
{
  currentX = point.point.GetX();
  // Если точка события - левый конец отрезка - возможно изменения статуса заметающей прямой.
  // добавляем отрезок в множество отрезков,точка которых лежит на заметающей прямой,
  // ищем верхнего и нижнего соседа этого отрезка на заметающей прямой проверяем на пересечение.
  if ( point.type == TypeEvent::Left )
  {
    auto it = currentSegments.insert( &point.s1 ).first;
    const LineData* lower = nullptr;

    if ( FindNeighborsLower(lower, currentSegments, it) )
    {
      IntersectLinesEvent( lower, &point.s1, intersectionPoints, setEventPoints, accuracyPolyliline );

    }
    const LineData* upper = nullptr;
    if ( FindNeighborUpper(upper, currentSegments, it) )
    {
      IntersectLinesEvent( &point.s1, upper, intersectionPoints, setEventPoints, accuracyPolyliline );
    }
  }
  else if ( point.type == TypeEvent::Right )
  {
    // В случае, если точка правая: находим у точки вехнего и нижнего соседа, проверяем их на возможное пересечение.
    // Удаляем отрезок из множества рассматриваемых отрезков.
    const LineData* lower = nullptr;
    const LineData* upper = nullptr;
    auto it = currentSegments.find( &point.s1 );
    if ( it == currentSegments.end() )
    {
      RestoreSanity( currentSegments );
      it = currentSegments.find( &point.s1 );
    }
    if ( FindNeighborsLower( lower, currentSegments, it ) && FindNeighborUpper( upper, currentSegments, it ) )
    {
      Point newPoint;
      if ( IntersectLines( upper->line, lower->line, newPoint ) )
      {
        if ( newPoint.GetX() > point.point.GetX() )
        {
          PointEvent event( newPoint, *upper, TypeEvent::Intersection );
          event.s2 = lower;
          setEventPoints.insert( event );
          if ( &upper->polyline != &lower->polyline )
            intersectionPoints.push_back( event );
        }
      }
      else if ( &upper->polyline != &lower->polyline &&
        ( fabs(Distance(upper->line, lower->line, newPoint)) < accuracyPolyliline )
                && newPoint.GetX() > point.point.GetX() )
      {
        PointEvent event( newPoint, *upper, TypeEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }
    currentSegments.erase( it );
  }
  else
  {// Если точка является точкой пересечения.
    currentX = 0.5 * ( oldPoint.GetX() + point.point.GetX() );
    // Смещаем координату x, относительно которой строится переупорядочиваение на половину расстояния до предыдущей точки события, ищем
    // отрезки, соответсвующие вехнему соседу верхнего отрезка, нижнему соседу нижнего отрезка, проверяем их на пересечение.

    const auto* s1 = &point.s1;
    const auto* s2 = point.s2;
    if ( s1 != nullptr && s2 != nullptr )
    {
      auto itS1 = currentSegments.find( &point.s1 );
      auto itS2 = currentSegments.find( point.s2 );
      if ( itS1 == currentSegments.end() || itS2 == currentSegments.end() )
      {
        RestoreSanity( currentSegments );
        itS1 = currentSegments.find( &point.s1 );
        itS2 = currentSegments.find( point.s2 );
      }
      const LineData* lower;
      const LineData* upper;
      if ( FindNeighborUpper( upper, currentSegments, itS1 ) && upper != s1 )
      {
        IntersectLinesEvent( point.s2, upper, intersectionPoints, setEventPoints, accuracyPolyliline );
      }
      if ( FindNeighborsLower( lower, currentSegments, itS2 ) )
      {
        Point newPoint;
        if ( ( IntersectLines( lower->line, point.s1.line, newPoint ) ) && &lower->polyline != &point.s1.polyline )
        {
          PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
          event.s2 = lower;
          setEventPoints.insert( event );
          intersectionPoints.push_back( event );
        }
        else if ( lower != s2 && std::abs( Distance( lower->line, s1->line, newPoint ) < 2. * accuracyPolyliline )
                  && &lower->polyline != &point.s1.polyline )
        {
          PointEvent event( newPoint, point.s1, TypeEvent::Intersection );
          event.s2 = lower;
          intersectionPoints.push_back( event );
        }
      }
      // После нахождения верхних и нижних соседей отрезки следует поменять местами относитльно currentX, 
      // являющейся половиной расстояния до следующей точки пересечения.
      if ( itS1 != currentSegments.end() && itS2 != currentSegments.end() && itS1 != itS2 )
      {
        s1 = *itS1;
        s2 = *itS2;
        currentSegments.erase( itS1 );
        currentSegments.erase( currentSegments.find( point.s2 ) );
        auto it = setEventPoints.begin();
        while ( it->point.GetX() <= point.point.GetX() )
          it++;
        auto shiftX = it->point.GetX();
        currentX = 0.5 * ( point.point.GetX() + shiftX );
        currentSegments.insert( s1 );
        currentSegments.insert( s2 );
      }
    }
  }

  // Записываем результат пересечения в виде: указатели на кривые, указатели на параметры кривых, соотвествующие пересечению.
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
   const auto s1Param = GetParamFromType( intersectionPoints[i].s1.curve, intersectionPoints[i].s1.leftParam, intersectionPoints[i].point );
   const auto s2Param = GetParamFromType( intersectionPoints[i].s2->curve, intersectionPoints[i].s2->leftParam, intersectionPoints[i].point );
      params.push_back( CurveIntersectionData(intersectionPoints[i].s1.curve, intersectionPoints[i].s2->curve,
                                               std::make_pair(s1Param, s2Param)) );
  }
  intersectionPoints.clear();
  if ( oldPoint.GetX() < point.point.GetX() )
    oldPoint = point.point;
}


//-----------------------------------------------------------------------------
//  Добавить в множество точек событий опорные точки полилинии.
// ---
static void CollectEventPoints( const std::vector<LineData>& lines, std::multiset<PointEvent, IsLexLessX>& setEventPoints )
{
  for ( size_t i = 0; i < lines.size(); ++i )
  { 
    setEventPoints.insert( PointEvent(lines[i].line.GetStartPoint(),lines[i], TypeEvent::Left) );
    setEventPoints.insert( PointEvent(lines[i].line.GetEndPoint(), lines[i], TypeEvent::Right) );
  }
}


//-----------------------------------------------------------------------------
//  Добавить в массив всех отрезков отрезки, полученые с полилиний.
// ---
static std::vector<LineData> CollectLines( const std::vector<Curve*>& curves )
{
  std::vector<GeomPolyline> polylines( curves.size() );
  for ( size_t i = 0; i < curves.size(); i++ )
  {
    if ( curves[i] != nullptr )
    {
      curves[i]->GetAsPolyLine( polylines[i] );
    }
    else
      polylines[i].Init( std::vector<Point>() );
  }
  std::vector<LineData> lines;
  for ( size_t j = 0; j < polylines.size(); j++ )
  {
    std::vector<Point> referensedPointsPolyline;
    polylines[j].GetReferensedPoints( referensedPointsPolyline );
    std::vector<double> refParams;
    curves[j]->GetReferensedParams( refParams );
    Point startPoint;
    Point endPoint;
    for ( size_t i = 0; i < referensedPointsPolyline.size() - 1; ++i )
    {
      if ( referensedPointsPolyline[i].GetX() < referensedPointsPolyline[i + 1].GetX() )
      {
        startPoint = referensedPointsPolyline[i];
        endPoint = referensedPointsPolyline[i + 1];
      }
      else
      {
        startPoint = referensedPointsPolyline[i + 1];
        endPoint = referensedPointsPolyline[i];
      }

      // Добавляем шум к концам отрезков. Необходимо для того, чтобы обрабатываеть случай, когда координаты точек пересечения совпадают
      // (вырожденный случай - одна пара отрезков должна быть упорядочена "по - новому", другая пара должна быть упорядочена "по старому".
      const auto urand1 = ( double ) rand() / ( double ) RAND_MAX;
      const auto urand2 = ( double ) rand() / ( double ) RAND_MAX;
      const auto urand3 = ( double ) rand() / ( double ) RAND_MAX;
      const auto urand4 = ( double ) rand() / ( double ) RAND_MAX;
      const auto lenght = ( startPoint - endPoint ).Lenght() * CommonConstantsMath::SCALING;
      const auto newStart = startPoint + Point( ( urand1 - 0.5 ) * lenght, ( urand2 - 0.5 ) * lenght );
      const auto newEnd = endPoint + Point( ( urand3 - 0.5 ) * lenght, ( urand4 - 0.5 ) * lenght );
      const Line line( newStart, newEnd );
      lines.push_back( LineData( line, polylines[j], refParams[i], *curves[j] ) );
    }
  }
  return lines;
}


//-----------------------------------------------------------------------------
//
// Используем алгоритм, построенный на основе заметающей прямой.
// Заметающая прямая - мысленно проведенная вертикальная прямая с координатой x = -бесконечность, которую перемещают вправо.
// Точка события - случай, когда возможно перечение отрезков. ( левый конец отрезка, правый конец отрезка, точка пересечения)
// По ходу своего движения эта прямая будет встречаться с отрезками, причём в любой момент времени каждый отрезок будет пересекаться с нашей прямой по одной точке.
// Таким образом,при появлении левого отрезка его точка появится на заметающей прямой и момент появления правого конца отрезка его точка удалится с прямой. 
// В каждый момент времени сохраняется относительный порядок отрезков, пересекающих заметающую прямую, они отсортированных по их y - координате.
// идея алгоритма в том, что при пересечении отрезков их точки будут иметь одинаковую y - координату на заметающей прямой.
// При фиксированном положении заметающей прямой необходимо рассматривать только соседние отрезки для нахождения пересечений.
//  Заметающую прямую необходимо рассматривать только в в следующих случаях: при появлении новых отрезков, удалении старых отрезков, а так же при нахождении пересечения 
//  отрезков.
// При появлении нового отрезка достаточно вставить его в нужное место в список, полученный для предыдущей сканирующей прямой. Проверять на пересечение надо только
// добавляемый отрезок с его непосредственными соседями в списке сверху и снизу.
// При исчезновении отрезка достаточно удалить его из текущего списка. После этого надо проверить на пересечение с верхним и нижним соседями в списке.
// Других изменений в порядке следования отрезков в списке, кроме описанных, не существует.Других проверок на пересечения производить не надо.
// Детальное описание алгоритма можно найти в reparata,Sheimos,Vychislitelnaya geometriya,1989гю стр 326.
// ---
static void SegmentsIntersections( std::vector<CurveIntersectionData>& params,
                                          const std::vector<Curve*>& curves, const double& accuracyPolyline )
{
  std::vector<PointEvent> intersectPoints;
  std::multiset<PointEvent, IsLexLessX> setEventPoints; // Множество отсортированных по х точек событий.
  std::set<const LineData*, KeySort> currentSegments;  // Множество отрезков, отсортированных по их положению на заметающей прямой.
  const auto lines = CollectLines( curves );
  CollectEventPoints( lines, setEventPoints );
  
  oldPoint = setEventPoints.begin()->point;
  while ( !setEventPoints.empty() )
  {
    PointEvent currentPoint = *setEventPoints.begin();

    setEventPoints.erase( setEventPoints.begin() );
    ProcessPoint( setEventPoints, currentSegments, currentPoint, intersectPoints, params, accuracyPolyline ); // Обработка текущей точки события.
  }
}



std::vector<CurveIntersectionData> Intersect( const std::vector<Curve*>& curves, const double&  accuracyPolyliline )
{
  std::vector<CurveIntersectionData> intersections;
  SegmentsIntersections( intersections, curves, accuracyPolyliline );
  // Находим начальные значения для метода Ньютона путем пересечения опроксимирующих кривые отрезков.
  std::vector<CurveIntersectionData> intersectPoints;

  for ( size_t i = 0; i < intersections.size(); i++ )
  {
    auto firstCurveParam = intersections[i].GetParams().first;
    auto secondCurveParam = intersections[i].GetParams().second;
    // Находим точку пересечения методом Ньютона.
    auto solutionExists = NewtonMethod( intersections[i].GetCurve1(), intersections[i].GetCurve2(), firstCurveParam,secondCurveParam );

    if ( solutionExists )
      intersectPoints.push_back( CurveIntersectionData(intersections[i].GetCurve1(), intersections[i].GetCurve2(), std::make_pair( firstCurveParam, secondCurveParam )) );
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
  const auto* currentLine = dynamic_cast< const Math::Line* >( &line );
  const auto* currentCircle = dynamic_cast< const Math::Ellipse* >( &circle );
  const auto p1 = currentLine->GetStartPoint();
  const auto p2 = currentLine->GetEndPoint();
  const auto sc = currentCircle->GetCenter();
  const auto r = currentCircle->GetMajorRadius();

  const auto dp = p2 - p1;

  const auto a = dp * dp;
  const auto b = 2. * ( dp.GetX() * (p1.GetX() - sc.GetX()) + dp.GetY() * (p1.GetY() - sc.GetY()) );
  auto c = sc * sc;
  c += p1 * p1;
  c -= 2. * ( sc * p1 );
  c -= r * r;
  const auto bb4ac = b * b - 4 * a * c;
  if ( fabs( a ) < CommonConstantsMath::NULL_TOL || bb4ac < 0 )
  {
    return result;
  }

  const auto mu1 = ( -b + sqrt(bb4ac)) / ( 2. * a );
  const auto mu2 = ( -b - sqrt(bb4ac)) / ( 2. * a );
  if ( mu1 == mu2 )
  {
    result.push_back( p1 + ( p2 - p1 ) * mu1 );
    return result;
  }
  else
  {
    auto point1 = p1 + ( p2 - p1 ) * mu1;
    if ( IsYinSegment( *currentLine, point1.GetY() ) && IsXinSegment( *currentLine, point1.GetX() ) )
      result.push_back( point1 );
    auto point2 = p1 + ( p2 - p1 ) * mu2;

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
  const auto* lineCurveFirst = dynamic_cast< const Math::GeomPolyline* >( &curve1 );
  std::vector<Point> referensedPointsPolyline1;
  lineCurveFirst->GetReferensedPoints( referensedPointsPolyline1 );
  const auto* lineCurveSecound = dynamic_cast< const Math::GeomPolyline* >( &curve2 );

  std::vector<double> refParams1;

  lineCurveFirst->GetReferensedParams( refParams1 );
  std::vector<double> refParams2;
  lineCurveSecound->GetReferensedParams( refParams2 );

  std::vector<Point> referensedPointsPolyline2;
  lineCurveSecound->GetReferensedPoints( referensedPointsPolyline2 );
  for ( size_t i = 1; i < referensedPointsPolyline1.size(); ++i )
  {
    Line firstLine( referensedPointsPolyline1[i - 1], referensedPointsPolyline1[i] );
    for ( size_t j = 1; j < referensedPointsPolyline2.size(); ++j )
    {
      Point point;
      if ( IntersectLines( Line( referensedPointsPolyline2[j - 1], referensedPointsPolyline2[j] ), firstLine, point ) )
      {
        auto result = std::find( std::begin( intersectPoints ), std::end( intersectPoints ), point );
        if ( result == std::end( intersectPoints ) )
        {
          intersectPoints.push_back( point );
          resultParams.push_back( CurveIntersectionData(curve1, curve2, std::make_pair(refParams1[i], refParams2[j])) );
        }
      }
    }
  }
  std::set<Point> s( intersectPoints.begin(), intersectPoints.end() );
  intersectPoints.assign( s.begin(), s.end() );
  return intersectPoints;
}




}
