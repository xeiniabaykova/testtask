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
// Найти значение матрицы вторых производных (матрицы Гессе) для квадрата расстояния между кривыми при параметрах (t1, t2).
// ---
static Matrix22 ComputeHessian( const Curve& curve1, const Curve& curve2, double t1, double t2 )
{
  Point t1Point;
  curve1.GetPoint( t1, t1Point );
  const auto x1 = t1Point.GetX();
  const auto y1 = t1Point.GetY();

  Vector t1PointD;
  curve1.GetDerivative( t1, t1PointD );
  const auto x1d = t1PointD.GetX();
  const auto y1d = t1PointD.GetY();

  Vector t1Point2D;
  curve1.Get2Derivative( t1, t1Point2D );
  const auto x1dd = t1Point2D.GetX();
  const auto y1dd = t1Point2D.GetY();

  Point t2Point;
  curve2.GetPoint( t2, t2Point );
  const auto x2 = t2Point.GetX();
  const auto y2 = t2Point.GetY();

  Vector t2PointD;
  curve2.GetDerivative( t2, t2PointD );
  const auto x2d = t2PointD.GetX();
  const auto y2d = t2PointD.GetY();

  Vector t2Point2D;
  curve2.Get2Derivative( t2, t2Point2D );
  const auto x2dd = t2Point2D.GetX();
  const auto y2dd = t2Point2D.GetY();
  // f(t1, t2) = (x1(t1) - x2(t2))^2 + (y1(t1) - y2(t2))^2
  // f''t1t1 = x1''(t1) * 2(x1(t1) - x2(t2)) + 2 * x1'(t1) + 2 * y1'(t1) + y1''(t1) * 2(y1(t1) - y2(t2))
  // f''t1t2 = 2 * x1'(t1) * x2'(t2) - 2*y1'(t1)*y2'(t2)
  // f''t2t2 = 2 * x2'(t2)*x2(t2) -2*x2''(t2)*(x1(t1) - x2(t2)) +2*y2'(t2)^2 +2*(y1(t1) -y2(t2))*y2''(t2)
  Matrix22 hessian;
  hessian[0][0] = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  hessian[0][1] = -2. * x2d * x1d - 2. * y2d * y1d;
  hessian[1][0] = -2. *x1d * x2d - 2. * y1d * y2d;
  hessian[1][1] = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;
  return hessian;
}


//-----------------------------------------------------------------------------
// Вычислить обратную матрицу и записать ее вместо исходной. Вернуть ссылку на неё же. Предполагается, что матрица невырождена.
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
// Вернуть градиент для квадрата расстояния между кривыми в заданной точке.
// ---
static Vector Gradient( const Curve& curve1, const Curve& curve2, const double& t1, const double& t2 )
{
  Point point1;
  curve1.GetPoint( t1, point1 );
  Vector grad1;
  curve1.GetDerivative( t1, grad1 );
  Point point2;
  curve2.GetPoint( t2, point2 );
  Vector grad2;
  curve2.GetDerivative( t2, grad2 );
  // f(t1, t2) = (x1(t1) - x2(t2))^2 + (y1(t1) - y2(t2))^2
  //f'(t1) = x1'(t1) * 2 * (x1(t1) - x2(t2) + y1'(t1) * (y1(t1) - y2(t2))
  //f'(t2) = -x2'(t2) * 2 * (x1(t1) - x2(t2) + y2'(t2) * (y1(t1) - y2(t2))
  const auto aResultT1 = 2.0 * ( point1.GetX() - point2.GetX() ) * grad1.GetX() + 2.0 * ( point1.GetY() - point2.GetY() ) * grad1.GetY();
  const auto aResultT2 = 2.0 * ( point2.GetX() - point1.GetX() ) * grad2.GetX() + 2.0 * ( point2.GetY() - point1.GetY() ) * grad2.GetY();
  return Vector( aResultT1, aResultT2 );
}


//-----------------------------------------------------------------------------
// Вернуть расстояние между двумя точками в параметрическом пространстве.
// ---
static double Distance( const double& paramCurve11, const double& paramCurve12, const double& paramCurve21, const double& paramCurve22 )
{
  return sqrt( (paramCurve11 - paramCurve21) * (paramCurve11 - paramCurve21) + (paramCurve12 - paramCurve22) * (paramCurve12 - paramCurve22) );
}


//-----------------------------------------------------------------------------
// Запустить метод Ньютона для поиска локального минимума квадрата расстояния между кривыми curve1, curve2
// с начальными значениями параметров кривых t1,t2.
// Если метод не сходится к решению, то возвращается false. Если сходится к решению, то возвращается true, а в параметрах
// t1,t2 возвращается решение.
// ---
static bool NewtonMethod( const Curve& curve1, const Curve& curve2, double& t1, double& t2 )
{
  auto currentParamCurve1 = t1;
  auto currentParamCurve2 = t2;
  Matrix22 lastNonDegenerateHessian = { 1., 0., 0., 1. };
  auto newParamCurve1 = t1;
  auto newParamCurve2 = t2;
  for ( size_t i = 0; i < CommonConstantsMath::MAX_NEWTON_STEPS; i++ )
  {
    auto hessian = ComputeHessian( curve1, curve2, currentParamCurve1, currentParamCurve2 );
    if ( fabs(hessian[0][0] * hessian[1][1] - hessian[0][1] * hessian[1][0]) > CommonConstantsMath::NULL_TOL )
      lastNonDegenerateHessian = hessian;
    else
      hessian = lastNonDegenerateHessian;

    auto invHessian = InverseMatrix( hessian );
    const auto grad = Gradient( curve1, curve2, currentParamCurve1, currentParamCurve2 );
    auto stepParam1 = -invHessian[0][0] * grad.GetX() - invHessian[0][1] * grad.GetY();
    auto stepParam2 = -invHessian[1][0] * grad.GetX() - invHessian[1][1] * grad.GetY();

    newParamCurve1 = currentParamCurve1 + stepParam1;
    newParamCurve2 = currentParamCurve2 + stepParam2;

    if ( fabs( Distance( newParamCurve1, newParamCurve2, currentParamCurve1, currentParamCurve2 ) ) < CommonConstantsMath::ACCURANCY_METHOD_NEWTON )
    {
      t1 = newParamCurve1;
      t2 = newParamCurve2;
      return true;
    }
    currentParamCurve1 = newParamCurve1;
    currentParamCurve2 = newParamCurve2;
  }
  return false;
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
// Проверить, пересекаются ли два заданных отрезка. Если отрезки пересекаются, вернуть true и в параметре intersectionPoint
// точку пересечения. Иначе вернуть false.
// ---
static bool IntersectLines( const Math::Line& line1, const Math::Line& line2, Point& intersectionPoint  )
{
  Vector dir1;
  line1.GetDerivative( 0. , dir1 );
  Vector dir2;
  line2.GetDerivative( 0., dir2 );
  const auto start1 = line1.GetStartPoint();
  const auto start2 = line2.GetStartPoint();
  const auto rX = start1.GetX() - start2.GetX();
  const auto rY = start1.GetY() - start2.GetY();
  if ( !dir1.IsCollinear(dir2) )
  {
    const auto det = (-dir2.GetX() * dir1.GetY() + dir1.GetX() * dir2.GetY());
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
// Отрезок и данные, необходимые для нахождения пересечения отрезков.Так же хранится указатель на полилинию:
// это необходимо, чтобы не учитывать перечечения отрезков внутри полилинии.
// ---
struct LineData
{
  const Line               line;      // Отрезок.
  const GeomPolyline&      polyline;  // Полилиния, которой принадлежит отрезок.
  const Curve&             curve;    // Кривая, соответствующая полилинии, которой принадлежит отрезок.
  const double             baseParam; // Параметр исходной кривой, соответсующий отрезку полилинии.
  LineData    ( const Line& theLine, const GeomPolyline& thePolyline, const double& theBaseParam, const Curve& theCurve ):
    line      ( theLine ),
    polyline  ( thePolyline ),
    baseParam ( theBaseParam ),
    curve     ( theCurve )
  {}
  LineData() = default;
};


//-----------------------------------------------------------------------------
// Вспомогательная структура для алгоритма быстрого пересечения отрезков.
// Точками события являются концы отрезков и точки пересечения.
// ---
struct PointEvent
{
  //-----------------------------------------------------------------------------
  // Перечисление для определения типа точки события:
  // Точка может быть левым концом отрезка, правым концом отрезка, точкой переcечения.
  // ---
  enum Type
  {
    Intersection, // Точка пересечения отрезков.
    Left,         // Левый конец отрезка.
    Right        // Правый конец отрезка.
  };
  Type            type;    // Тип точки события.
  const Point     point;   // Координаты точки.
  const LineData  &s1;     // Отрезок, соответствующий точке события.
  const LineData  *s2;     // Если точка события является точкой пересечения, то храним второй отрезок тоже.
  PointEvent ( const Point& thePoint, const LineData &theS1, const Type& theType ):
    point    ( thePoint ),
    type     ( theType ),
    s1       ( theS1 ),
    s2       ( nullptr )
  {}
};


//-----------------------------------------------------------------------------
// Проверить точки на точное равенство. 
// ---
static bool IsSame( const Point& lhs, const Point& rhs )
{
  return ( lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY() );
}


//-----------------------------------------------------------------------------
// Сравнить точки лексикографически. X старше y.
// ---
static bool IsLexLessFromX( const Point& lhs, const Point& rhs )
{
  if ( lhs.GetX() < rhs.GetX() )
    return true;
  else if ( lhs.GetX() == rhs.GetX() )
    return ( lhs.GetY() < rhs.GetY() );
  else
    return false;
}


//-----------------------------------------------------------------------------
// Функтор лексиграфического сравнения точек, x старше y. 
// ---
struct IsLexLessX
{
  // lhs < rhs
  inline bool operator()( const PointEvent &lhs, const PointEvent& rhs ) const
  {
    if ( IsSame(lhs.point, rhs.point) )
      return IsLexLessFromX( lhs.s1.line.GetEndPoint(), rhs.s1.line.GetEndPoint() );
    return IsLexLessFromX( lhs.point, rhs.point );
  }
};


//-----------------------------------------------------------------------------
// Получить по координате х, принадлежащей отрезку, соответсвующую координату y.
// ---
double GetYFromX( const Line& line, const double& x )
{
  const auto x1 = line.GetStartPoint().GetX();
  const auto y1 = line.GetStartPoint().GetY();
  const auto x2 = line.GetEndPoint().GetX();
  const auto y2 = line.GetEndPoint().GetY();
  if ( fabs(x1 - x2) < CommonConstantsMath::NULL_TOL )
    return y2;
  return y1 + ( y2 - y1 )*( x - x1 ) / ( x2 - x1 );
}


//-----------------------------------------------------------------------------
// Функтор упорядочивания отрезков по значению y для данного x.
// (сравниваются всегда отрезки, которые содержат точку с этим x).
//--
struct LineComparator
{
  static double currentX; // Текущая координата х.
  inline bool operator () ( const LineData* lhs, const LineData* rhs ) const
  {
    const auto lhsY = GetYFromX( lhs->line, currentX );
    const auto rhsY = GetYFromX( rhs->line, currentX );
    // Если координаты y совпадают (случай нескольких отрезков, исходящих из одной точки), то упорядочивание происходит по 
    // y - координате конца отрезка.
    if ( lhsY == rhsY )
      if ( IsSame(lhs->line.GetStartPoint(), rhs->line.GetStartPoint()) )
        return GetYFromX( lhs->line, lhs->line.GetEndPoint().GetX() ) < GetYFromX( rhs->line, lhs->line.GetEndPoint().GetX() );
    //  else 
    return ( lhsY < rhsY );
  }

};
double LineComparator::currentX;

using ActiveLines = std::set<const LineData*, LineComparator>;
//-----------------------------------------------------------------------------
// Получить верхнего соседа для отрезка, заданного итератором it. 
// Если соседа нет, возвращаем false, если сосед есть, возвращаем true, соседа в поле upper.
// ---
static bool FindNeighborUpper( const ActiveLines& setProcessedSegments, ActiveLines::iterator it, const LineData*& upper )
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
// Получить нижнего соседа для отрезка, заданного итератором it. 
// Если соседа нет, возвращаем false, если сосед есть, возвращаем true, соседа в поле lower.
// ---
static bool FindNeighborsLower( const ActiveLines& setProcessedSegments, ActiveLines::iterator it, const LineData*& lower )
{
  if ( --it != setProcessedSegments.end() )
  {
    lower = *it;
    return true;
  }
  else
    return false;
}

Point oldPoint; // Точка события с предыдущим значением х. Необходима для того, чтобы при пересечении находить нужную координату упорядочивания "До" точки пересечения.

//-----------------------------------------------------------------------------
// Преобразовать упорядочивание к паравильному относительно текущего x.
// Установленное упорядочивание отрезков может быть неправильным из-за накопившеся числовой ошибки.
// ---
void RestoreSanity( ActiveLines& currentSegments )
{
  ActiveLines newSet;
  for ( auto it = currentSegments.begin(); it != currentSegments.end(); it++ )
    newSet.insert( *it );
  currentSegments = newSet;
}


//-----------------------------------------------------------------------------
// В случае, когда исходная кривая отрезок или полилиния, возвращаем параметр, 
// соотвествующий точке. В другом случае возвращаем ближайшую границу параметра.
// ---
double RefineParameter( const Curve& intersectCurve, const double& baseParam, const Point& point )
{
  if ( intersectCurve.GetType() == Curve::CurveType::LineType )
    return static_cast<const Line*>( &intersectCurve )->GetTFromPoint( point );
  else 
    if ( intersectCurve.GetType() == Curve::CurveType::PolylineType )
      return static_cast<const GeomPolyline*>( &intersectCurve )->GetTFromPoint( point );
  else 
    return baseParam;
}


//-----------------------------------------------------------------------------
// Проверить пересечения отрезков. Если отрезки пересекаются и полилинии совпадают, то заносим точку пересечений в множество событий,
// но не добавляем к множеству точек пересечения. Если же отрезки не пересекаются, но находятся на достаточно близком расстоянии
// друг от друга, то не добавляем эту точку в множество событий, но добавляем к множеству точек пересечения.
// ---
void CheckIntersection( const LineData* line1, const LineData* line2, std::vector<PointEvent>& intersectionPoints,
                          std::set<PointEvent, IsLexLessX>& setEventPoints, const double& accuracyPolyliline )
{
  if ( line1 != nullptr && line2 != nullptr )
  {
    Point newPoint;
    if ( IntersectLines(line2->line, line1->line, newPoint) )
    {
      PointEvent event( newPoint, *line2, PointEvent::Intersection );
      event.s2 = line1;
      setEventPoints.insert( event );
      if ( &line1->polyline != &line2->polyline )
        intersectionPoints.push_back( event );
    }
    else 
      if ( &line1->polyline != &line2->polyline &&
           ( fabs(Distance(line1->line, line2->line, newPoint)) < 2. * accuracyPolyliline) )
      {
        PointEvent event( newPoint, *line2, PointEvent::Intersection );
        event.s2 = line1;
        intersectionPoints.push_back( event );
      }
  }
}


//-----------------------------------------------------------------------------
// Обработать точку события.
// ---
static void ProcessPoint( std::set<PointEvent, IsLexLessX>& setEventPoints, ActiveLines& currentSegments, const PointEvent& point,std::vector<PointEvent>& intersectionPoints,
	Intersections& intesectPoints, const double& accuracyPolyliline )
{
  LineComparator::currentX = point.point.GetX();
  // Если точка события - левый конец отрезка - возможно изменения статуса заметающей прямой.
  // добавляем отрезок в множество отрезков,точка которых лежит на заметающей прямой,
  // ищем верхнего и нижнего соседа этого отрезка на заметающей прямой проверяем на пересечение.
  if ( point.type == PointEvent::Left )
  {
    auto it = currentSegments.insert( &point.s1 ).first; // добавление отрезка в множество отрезков, точки которых лежат на заметающей прямой.
    const LineData* lower = nullptr;

    if ( FindNeighborsLower(currentSegments, it, lower) )
    {
      CheckIntersection( lower, &point.s1, intersectionPoints, setEventPoints, accuracyPolyliline );
    }
    const LineData* upper = nullptr;
    if ( FindNeighborUpper( currentSegments, it, upper) )
    {
      CheckIntersection( &point.s1, upper, intersectionPoints, setEventPoints, accuracyPolyliline );
    }
  }
  else if ( point.type == PointEvent::Right )
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
    if ( FindNeighborsLower( currentSegments, it, lower ) && FindNeighborUpper( currentSegments, it, upper ) )
    {
      Point newPoint;
      if ( IntersectLines( upper->line, lower->line, newPoint ) )
      {
        if ( newPoint.GetX() > point.point.GetX() )
        {
          PointEvent event( newPoint, *upper, PointEvent::Intersection );
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
        PointEvent event( newPoint, *upper, PointEvent::Intersection );
        event.s2 = lower;
        intersectionPoints.push_back( event );
      }
    }
    currentSegments.erase( it );
  }
  else
  {// Если точка является точкой пересечения.
    LineComparator::currentX = 0.5 * ( oldPoint.GetX() + point.point.GetX() );
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
      if ( FindNeighborUpper( currentSegments, itS1, upper ) && upper != s1 )
      {
        CheckIntersection( point.s2, upper, intersectionPoints, setEventPoints, accuracyPolyliline );
      }
      if ( FindNeighborsLower( currentSegments, itS2, lower ) )
      {
        Point newPoint;
        if ( ( IntersectLines( lower->line, point.s1.line, newPoint ) ) && &lower->polyline != &point.s1.polyline )
        {
          PointEvent event( newPoint, point.s1, PointEvent::Intersection );
          event.s2 = lower;
          setEventPoints.insert( event );
          intersectionPoints.push_back( event );
        }
        else if ( lower != s2 && std::fabs( Distance( lower->line, s1->line, newPoint ) < 2. * accuracyPolyliline )
                  && &lower->polyline != &point.s1.polyline )
        {
          PointEvent event( newPoint, point.s1, PointEvent::Intersection );
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
        LineComparator::currentX = 0.5 * ( point.point.GetX() + shiftX );
        currentSegments.insert( s1 );
        currentSegments.insert( s2 );
      }
    }
  }

  // Записываем результат пересечения в виде: указатели на кривые, указатели на параметры кривых, соотвествующие пересечению.
  for ( size_t i = 0; i < intersectionPoints.size(); ++i )
  {
   const auto s1Param = RefineParameter( intersectionPoints[i].s1.curve, intersectionPoints[i].s1.baseParam, intersectionPoints[i].point );
   const auto s2Param = RefineParameter( intersectionPoints[i].s2->curve, intersectionPoints[i].s2->baseParam, intersectionPoints[i].point );
   std::pair<const Curve*, const Curve*> curves;
   if ( &intersectionPoints[i].s1.curve > &intersectionPoints[i].s2->curve )
	   curves = std::make_pair( &intersectionPoints[i].s2->curve, &intersectionPoints[i].s1.curve );
   else
	   curves = std::make_pair( &intersectionPoints[i].s1.curve, &intersectionPoints[i].s2->curve );

   intesectPoints[curves].push_back( std::make_pair( s1Param, s2Param) );
  }
  intersectionPoints.clear();
  if ( oldPoint.GetX() < point.point.GetX() )
    oldPoint = point.point;
}


//----------------------------------------------------------------------------
//  Добавить в множество точек событий крайние точки отрезков.
// ---
static void CollectEventPoints( const std::vector<LineData>& lines, std::set<PointEvent, IsLexLessX>& setEventPoints )
{
  for ( size_t i = 0; i < lines.size(); ++i )
  { 
    setEventPoints.insert( PointEvent(lines[i].line.GetStartPoint(),lines[i], PointEvent::Left) );
    setEventPoints.insert( PointEvent(lines[i].line.GetEndPoint(), lines[i], PointEvent::Right) );
  }
}


//-----------------------------------------------------------------------------
// Добавить в массив всех отрезков отрезки, полученые с полилиний.
// ---
static void CollectLines( const std::vector<Curve*>& curves, std::vector<LineData>& lines )
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
  for ( size_t j = 0; j < polylines.size(); j++ )
  {
    std::vector<Point> referencePoints;
    polylines[j].GetReferensedPoints( referencePoints );
    std::vector<double> refParams;
    curves[j]->GetReferensedParams( refParams );  
    for ( size_t i = 0; i < referencePoints.size() - 1; ++i )
    {
      Point startPoint;
      Point endPoint;
      if ( referencePoints[i].GetX() < referencePoints[i + 1].GetX() )
      {
        startPoint = referencePoints[i];
        endPoint = referencePoints[i + 1];
      }
      else
      {
        startPoint = referencePoints[i + 1];
        endPoint = referencePoints[i];
      }

      // Добавляем шум к концам отрезков. Алгоритм не учитывает вырожденные случаи - когда концы отрезков совпадают, когда несколько отрезков пересекаются в одной точке.
      // Для того, чтобы бороться с вырожденными случаями добавляем небольшой шум на концах отрезков для того, чтобы получить какое - нибудь упорядочивание.
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
}


//-----------------------------------------------------------------------------
// Для нахождения пересечений отрезков полилиний используется алгоритм на основе метода заметающей прямой.
// Он позволяет найти все точки пересечения без перебора всевозможных пар отрезков. 
// Заметающая прямая - мысленно проведенная вертикальная прямая, которая перемещается слева направо.
// В каждый момент времени заметающая прямая определяет порядок на подмножестве отрезков, которые она пересекают, по y-координате точек пересечения.
// Изменение этого подмножества отрезков и упорядочивания происходит при достижении точек событий - левых и правых по x концов отрезков и точек пересечения.
// Используются 2 основных структуры данных: упорядоченный набор точек событий и текущее подмножество отрезков (активные отрезки).
// Точки событий упорядочиваются лексикографически (x старше y) и обрабатываются по возрастанию, что соответствует движению заметающей прямой слева направо.
// При обработке левого конца отрезка он добавляется к подмножеству активных, проверяются пересечения с верхним и нижним соседом в активном множестве.
// В случае обнаружения пересечения правее текущей точки, создается соответствующая точка события и добавляется в множество.
// При обрабботке правого конца отрезка он удаляется из подмножества активных, проверяются и обрабатываются пересечения с верхним и нижним соседом.
// При обработке точки пересечения меняется порядок отрезков в активном множестве, проверяются и обрабатываются пересечения с соседями в активном множестве.
// Теоретически доказано, что при такой схеме обработки будут найдены все точки пересечения. 
// Реализация выполнена на основе книги Препарата, Шеймос "Вычислительная геометрия: Введение", там приведено подробное описание алгоритма.
// ---
static void SegmentsIntersections( const std::vector<Curve*>& curves,
                                   Intersections &intersectionPoints, const double& accuracyPolyline )
{
  std::vector<PointEvent> intersectPoints;
  std::set<PointEvent, IsLexLessX> setEventPoints; // Множество отсортированных по х точек событий.
  ActiveLines currentSegments; // Множество отрезков, отсортированных по их положению на заметающей прямой.
  std::vector<LineData> lines;
  CollectLines( curves, lines ); // в массиве Lines - находятся все отрезки, для которых ищем пересечение.
  CollectEventPoints( lines, setEventPoints );// Добавляем левые и правые концы в точки событий.
  
  oldPoint = setEventPoints.begin()->point;// Начальное состояние заметающей прямой.   
  while ( !setEventPoints.empty() )
  {
    PointEvent currentPoint = *setEventPoints.begin();
    setEventPoints.erase( setEventPoints.begin() );
    ProcessPoint( setEventPoints, currentSegments, currentPoint, intersectPoints, intersectionPoints, accuracyPolyline );
  }
}


//-----------------------------------------------------------------------------
//  Найти пересечения кривых.
// ---
void Intersect( const std::vector<Curve*>& curves, Intersections& resultIntersectionPoints, const double& accuracyPolyliline )
{
  Intersections intersectionPoints;
  SegmentsIntersections( curves, intersectionPoints, accuracyPolyliline );
  // Находим начальные значения для метода Ньютона путем пересечения опроксимирующих кривые отрезков.

  for ( auto it = intersectionPoints.begin(); it != intersectionPoints.end(); it++ )
  {
    std::vector<std::pair<double, double>> params = it->second;
    for ( int i = 0; i < params.size(); i++ )
    {
      auto firstCurveParam = params[i].first;
      auto secondCurveParam = params[i].second;
      // Находим точку пересечения методом Ньютона.
      auto solutionExists = NewtonMethod( *(it->first.first), *it->first.second, firstCurveParam, secondCurveParam );

      if ( solutionExists )
      {
        resultIntersectionPoints[it->first].push_back( std::make_pair( firstCurveParam, secondCurveParam ) );
      }
    }
  }

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
//static std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2, std::vector<CurveIntersectionData>& resultParams )
//{
//  std::vector<Point> intersectPoints;
//  const auto* lineCurveFirst = dynamic_cast< const Math::GeomPolyline* >( &curve1 );
//  std::vector<Point> referensedPointsPolyline1;
//  lineCurveFirst->GetReferensedPoints( referensedPointsPolyline1 );
//  const auto* lineCurveSecound = dynamic_cast< const Math::GeomPolyline* >( &curve2 );
//
//  std::vector<double> refParams1;
//
//  lineCurveFirst->GetReferensedParams( refParams1 );
//  std::vector<double> refParams2;
//  lineCurveSecound->GetReferensedParams( refParams2 );
//
//  std::vector<Point> referensedPointsPolyline2;
//  lineCurveSecound->GetReferensedPoints( referensedPointsPolyline2 );
//  for ( size_t i = 1; i < referensedPointsPolyline1.size(); ++i )
//  {
//    Line firstLine( referensedPointsPolyline1[i - 1], referensedPointsPolyline1[i] );
//    for ( size_t j = 1; j < referensedPointsPolyline2.size(); ++j )
//    {
//      Point point;
//      if ( IntersectLines( Line( referensedPointsPolyline2[j - 1], referensedPointsPolyline2[j] ), firstLine, point ) )
//      {
//        auto result = std::find( std::begin( intersectPoints ), std::end( intersectPoints ), point );
//        if ( result == std::end( intersectPoints ) )
//        {
//          intersectPoints.push_back( point );
//          resultParams.push_back( CurveIntersectionData(curve1, curve2, std::make_pair(refParams1[i], refParams2[j])) );
//        }
//      }
//    }
//  }
//  std::set<Point> s( intersectPoints.begin(), intersectPoints.end() );
//  intersectPoints.assign( s.begin(), s.end() );
//  return intersectPoints;
//}




}
