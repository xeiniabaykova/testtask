#include "Intersector.h"
#include "Line.h"
#include "Ellipse.h"
#include "GeomPolyline.h"
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <set>
namespace Math {
namespace {

//-----------------------------------------------------------------------------
//  Найти значение матрицы Гессе для квадрата расстояния между кривыми в точке (t1, t2).
// ---
std::vector<std::vector<double>> CountingHessian( const Curve& curve1, const Curve& curve2, double t1, double t2 )
{
  const double x1 = curve1.GetPoint( t1 ).GetX();
  const double y1 = curve1.GetPoint( t1 ).GetY();
  const double x1d = curve1.GetDerivative( t1 ).GetX();
  const double y1d = curve1.GetDerivative( t1 ).GetY();
  const double x1dd = curve1.Get2Derivative( t1 ).GetX();
  const double y1dd = curve1.Get2Derivative( t1 ).GetY();

  const double x2 = curve2.GetPoint( t2 ).GetX();
  const double y2 = curve2.GetPoint( t2 ).GetY();
  const double x2d = curve2.GetDerivative( t2 ).GetX();
  const double y2d = curve2.GetDerivative( t2 ).GetY();
  const double x2dd = curve2.Get2Derivative( t2 ).GetX();
  const double y2dd = curve2.Get2Derivative( t2 ).GetY();

  const double F11 = x1dd * 2. * ( x1 - x2 ) + 2. * x1d * x1d + y1dd * 2. * ( y1 - y2 ) + 2. * y1d * y1d;
  const double F12 = -2. * x2d * x1d - 2. * y2d * y1d;
  const double F21 = -2. *x1d * x2d - 2. * y1d * y2d;
  const double F22 = -2. * ( x1 - x2 ) * x2dd + 2.* x2d * x2d - 2. * ( y1 - y2 ) * y2dd + 2. * y2d * y2d;

  std::vector<std::vector <double>> hessian;
  hessian.resize( 2 );
  hessian[0].resize( 2 );
  hessian[1].resize( 2 );
  hessian[0][0] = F11;
  hessian[0][1] = F12;
  hessian[1][0] = F21;
  hessian[1][1] = F22;
  return hessian;

}


//-----------------------------------------------------------------------------
//  Найти обратную матрицу для матрицы 2*2.
// ---
void InverseMatrix( std::vector<std::vector<double>>& matrix )
{
  const double d = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  const double a00 = matrix[1][1] / d;
  const double a01 = -matrix[0][1] / d;
  const double a10 =  - matrix[1][0] / d;
  const double a11 = matrix[0][0] / d;
  matrix[0][0] = a00;
  matrix[0][1] = a01;
  matrix[1][0] = a10;
  matrix[1][1] = a11;
}


//-----------------------------------------------------------------------------
//  Найти градиент для квадрата расстояния между кривыми в точке start.
// ---
Point Gradient( const Curve& curve1, const Curve& curve2, Point start )
{
  Point aPoint1 = curve1.GetPoint(start.GetX());
  Vector aGrad1 = curve1.GetDerivative(start.GetX());

  Point aPoint2 = curve2.GetPoint( start.GetY() );
  Vector aGrad2 = curve2.GetDerivative(start.GetY());
  double aResultT1 = 2.0 * ( aPoint1.GetX()- aPoint2.GetX() ) * aGrad1.GetX()+ 2.0 * ( aPoint1.GetY()- aPoint2.GetY() ) * aGrad1.GetY();
  double aResultT2 = 2.0 * ( aPoint2.GetX() - aPoint1.GetX() ) * aGrad2.GetX() + 2.0 * ( aPoint2.GetY()- aPoint1.GetY() ) * aGrad2.GetY();
  return Point(aResultT1, aResultT2);
}


//-----------------------------------------------------------------------------
//  Провести одну итерацию метода Ньютона.
// ---
Point NewtonMethodIteration(const Curve& curve1, const Curve& curve2, Point startValue)
{
  std::vector<std::vector<double>> hessian = CountingHessian( curve1, curve2, startValue.GetX(), startValue.GetY() );
  InverseMatrix( hessian );
  Point grad = Gradient( curve1, curve2, startValue );
  Point temp( hessian[0][0] * -grad.GetX() + hessian[0][1] * -grad.GetY(), hessian[1][0] * -grad.GetX() + hessian[1][1] * -grad.GetY() );

  return  startValue + temp;
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
//  Найти пересечение двух отрезков, лежащих на одной прямой.
// ---
ptrdiff_t FindIntersection( double u0, double u1, double  v0, double v1, double* w )
{
  if( u1 < v0 || u0 > v1 )
    return 0;
  if ( u1 > v0 )
  {
    if ( u0 < v1 )
    {
      if (u0 < v0)
        w[0] = v0;
      else
        w[0] = u0;
      if ( u1 > v1 )
        w[1] = v1;
      else
        w[1] = u1;
      return 2;
    } else
    {
      w[0] = u0;
      return 1;
    }
  }
  else
  {
    w[0] = u1;
    return 1;
  }
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух отрезков.
// ---
bool IntersectLines( const Curve& curve1, const Curve& curve2, Point &thePoint )
{
  std::vector<Point> IntersectPoints;
  const Math::Line* lineCurveFirst =  dynamic_cast<const Math::Line*>( &curve1 );
  const Math::Line* lineCurveSecound =  dynamic_cast<const Math::Line*>( &curve2 );
  if ((fabs(lineCurveFirst->GetStartPoint().GetX()) > 10e7)
    || (fabs(lineCurveFirst->GetStartPoint().GetY()) > 10e7)
    || (fabs(lineCurveSecound->GetStartPoint().GetX()) > 10e7)
    || (fabs(lineCurveSecound->GetStartPoint().GetY()) > 10e7))
    return false;

  

  const Point P0 = lineCurveFirst->GetStartPoint();
  const Vector V0 = lineCurveFirst->GetEndPoint() - lineCurveFirst->GetStartPoint();

  const Point P1 = lineCurveSecound->GetStartPoint();
  const Vector V1 = lineCurveSecound->GetEndPoint() - lineCurveSecound->GetStartPoint();

  Vector distanceStart = P1 - P0;
  const double krossDirecting = V0.VectorMult( V1 );
  const double sqrKross = krossDirecting * krossDirecting;
  const double sqrLen0 = V0.Lenght();
  const double sqrLen1 = V1.Lenght();
  if ( !V0.IsCollinear(V1) )
  {

    const double s = distanceStart.VectorMult( V1 ) / krossDirecting;
    if( s < 0 || s > 1 )
    {
      return false;
    }
    const double t = distanceStart.VectorMult( V0 ) / krossDirecting;
    if( t < 0 || t > 1)
    {
      return false;
    }
    thePoint = lineCurveFirst->GetPoint(s);
    return true;

 } else
  {
    const double sqrLenE = distanceStart.Lenght() * distanceStart.Lenght();
    const double kross = distanceStart.VectorMult( V0 );
    double  sqrKross = kross * kross;
    if ( sqrKross > sqrLen0 * sqrLenE *CommonConstantsMath::NULL_TOL )
    {
      return false;
    }
    const double s0 = V0 * distanceStart / sqrLen0;
    const double s1 = ( s0 + V0 * V1 )/sqrLen0;
    double w[2];
    const double smin = std::min( s0, s1 );
    const double smax = std::max( s0, s1 );
    size_t imax = FindIntersection( 0.0, 1.0, smin, smax, w );
    for ( size_t i = 0; i < imax; ++i )
      IntersectPoints.push_back( P0 + V0 * w[i] );
     thePoint = IntersectPoints[0];
     return false;
  }

}


void FindSegments( std::set<PointEvent,SortByX>& Q, Point testPoint, std::vector<PointEvent>& L, std::vector<PointEvent>& R, std::vector<PointEvent>& I )
{
  L.clear();
  R.clear();
  I.clear();
  for (auto it = Q.begin();it !=Q.end(); it++)
  {
    if ( ((fabs(it->point.GetX() - testPoint.GetX())) < CommonConstantsMath::NULL_TOL )
      && it->point.GetX() < it->otherPoint.GetX() )
      L.push_back(*it);
    if (((fabs(it->point.GetX() - testPoint.GetX())) < CommonConstantsMath::NULL_TOL)
      && it->point.GetX() > it->otherPoint.GetX())
      R.push_back(*it);
    if (it->numOfSegment == -1)
      I.push_back(*it);
  }

}

PointEvent FindUpper(const std::set<PointEvent>& T)
{
  PointEvent event(Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()),
    Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), 0, typeEvent::NonValid );
  for (auto it = T.begin(); it != T.end(); it++)
  {
    if (it->point.GetY() > event.point.GetY())
      event = *it;
  }
  return event;
}
PointEvent FindLower(const std::set<PointEvent>& T)
{
  PointEvent event(Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()),
    Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), 0, typeEvent::NonValid );
  for (auto it = T.begin(); it != T.end(); it++)
  {
    if (it->point.GetY() < event.point.GetY())
      event = *it;
  }
  return event;

}

bool FindAbscissa( PointEvent thePoint, std::multiset<PointEvent, SortByX>& Q )
{
 
  for (auto it = Q.begin(); it != Q.end(); it++)
  {
    if (IsEqual(it->point, thePoint.point))
      return true;
  }
  return false;

}
void FindNeighbors( PointEvent& upper, PointEvent& lower, const std::set<PointEvent>& T,const PointEvent& point )
{
  upper.point = Point(point.point.GetX(), std::numeric_limits<double>::lowest());
  lower.point = Point(point.point.GetX(), std::numeric_limits<double>::max());
  for (auto it = T.begin(); it != T.end(); it++)
  {
    if (it->point.GetY() < upper.point.GetY() && it->point.GetY() > point.point.GetY())
      upper = *it;
    if (it->point.GetY() > lower.point.GetY() && it->point.GetY() < point.point.GetY())
      lower = *it;
  }
}
bool IsXinSegment(const Line& line, double x)
{
  if (line.GetStartPoint().GetX() > line.GetEndPoint().GetX())
    return (line.GetEndPoint().GetX() <= x && x <= line.GetStartPoint().GetX());
  else
    return (line.GetEndPoint().GetX() >= x && x >= line.GetStartPoint().GetX());
}

double GetYFromX(const Line& line, double x)
{
  double x1 = line.GetStartPoint().GetX();
  double y1 = line.GetStartPoint().GetY();

  double x2 = line.GetEndPoint().GetX();
  double y2 = line.GetEndPoint().GetY();

  double ch = -(x1*y2 - x2*y1) - x*(y1 - y2);
  return ch / (x2 - x1);
}

bool FindNeighborUpper(Line& upper, const std::vector<Line>& T, const PointEvent& point)
{
  bool result = false;
  upper = Line(Point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()),
    Point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()));
  Line currentLine(point.point, point.otherPoint);
  for (auto it = T.begin(); it != T.end(); it++)
  {
    if (IsXinSegment(*it, point.point.GetX()))
    {
      
      double currentY = GetYFromX(*it, point.point.GetX());
      if (currentY >= upper.GetStartPoint().GetY() && currentY >= point.point.GetY()
        && !(currentLine == *it ))
      {
        upper = *it;
        result = true;
      }
    }
  }
  return result;
}

bool FindNeighborsLower( Line& lower, const std::vector<Line>& T, const PointEvent& point )
{
  bool result = false;
  lower = Line( Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()),
    Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()) );
  for ( auto it = T.begin(); it != T.end(); it++ )
  {
    if ( IsXinSegment(*it, point.point.GetX()) )
    {
    
      double currentY = GetYFromX(*it, point.point.GetX());
      Line currentLine(point.point, point.otherPoint);
      if (currentY <= lower.GetStartPoint().GetY() && currentY <= point.point.GetY()
        && !(currentLine == *it))
      {
        lower = *it;
        result = true;
      }
    }
  }
  return result;
}
void ProcessPoint(std::multiset<PointEvent, SortByX>& Q, std::vector<Line>& T, std::vector<PointEvent>&L,
  std::vector<PointEvent>&R, std::vector<PointEvent>&I, PointEvent point, std::vector<PointEvent>& intersectionPoints,
  std::vector<Point>& result )
{
  if (point.type == typeEvent::Left)
  {
    T.push_back(Line(point.point, point.otherPoint));
    Line lower;

    if (FindNeighborsLower(lower, T, point))
    {
      Point newPoint;
      if (IntersectLines(lower, Line(point.point, point.otherPoint), newPoint))
      {
        PointEvent event(newPoint, newPoint, 0, typeEvent::Intersection);
        event.s1 = Line(point.point, point.otherPoint);
        event.s2 = lower;
        intersectionPoints.push_back(event);
      }
    }
    Line upper;
    if (FindNeighborUpper(upper, T, point))
    {
      Point newPoint;
      if (IntersectLines(upper, Line(point.point, point.otherPoint), newPoint))
      {
        PointEvent event(newPoint, newPoint, 0, typeEvent::Intersection);
        event.s1 = upper;
        event.s2 = Line(point.point, point.otherPoint);
        intersectionPoints.push_back(event);
      }
    }

  }
  else if (point.type == typeEvent::Right)
  {
    Line lower;
    Line upper;
    if ( FindNeighborsLower(lower, T, point) && FindNeighborUpper(upper, T, point))
    {
      Point newPoint;
      if (IntersectLines(upper,lower, newPoint))
      {
        if (newPoint.GetX() > point.point.GetX())
        {
          PointEvent event(newPoint, newPoint, 0, typeEvent::Intersection);
          intersectionPoints.push_back(event);
        }

      }
    }

  }
  else
  {
    Line s1 = point.s1;
    Line s2 = point.s2;
    Line lower;
    Line upper;
    if ( FindNeighborsLower(lower, T, PointEvent(s1.GetStartPoint(), s1.GetEndPoint(), 0, typeEvent::Left)) )
    {
      Point newPoint;
      if (IntersectLines(lower, s1, newPoint))
      {
        PointEvent event(newPoint, newPoint, 0, typeEvent::Intersection);
        intersectionPoints.push_back(event);
      }
    }
    if ( FindNeighborUpper(upper, T, PointEvent(s2.GetStartPoint(), s2.GetEndPoint(), 0, typeEvent::Left)))
    {
      Point newPoint;
      if (IntersectLines(upper, s2, newPoint))
      {
        PointEvent event(newPoint, newPoint, 0, typeEvent::Intersection);
        intersectionPoints.push_back(event);
      }
    }
    auto it = std::find(T.begin(), T.end(), s1);
    auto it2 = std::find(T.begin(), T.end(), s2);
    if ( it != T.end() && it2 != T.end() )
      std::iter_swap(it, it2);

  }
  for (int i = 0; i < intersectionPoints.size(); i++)
  {
    result.push_back(intersectionPoints[i].point);
    if (!FindAbscissa(intersectionPoints[i], Q))
    {  
      Q.insert(intersectionPoints[i]);
   }
  }
  intersectionPoints.clear();
}


std::vector<Point> segmentsIntersections( std::vector<Line> segments )
{
  std::vector<Point> result;
  std::vector<PointEvent> intersectPoints;

  std::multiset<PointEvent, SortByX> Q;
  std::vector<Line> T;
  std::vector<PointEvent> L;
  std::vector<PointEvent>R;
  std::vector<PointEvent> I;

  for (int i = 0; i < segments.size(); i++)
  {
    Q.insert( PointEvent(segments[i].GetStartPoint(), segments[i].GetEndPoint(),i, typeEvent::Left) );
    Q.insert( PointEvent(segments[i].GetEndPoint(), segments[i].GetStartPoint(), i, typeEvent::Right) );
  }

 // 1) Инициализируются Q и T.В Q заносятся все концы отрезков, упорядоченные по координате x, при этом, если две точки совпали,
//  то левая конечная точка отрезка помещается перед правой.Если совпали только x, то точка с меньшим y является меньшей.
 //   T ← ∅

  while ( !Q.empty() )
  {
    PointEvent currentPoint = *Q.begin();
    
    Q.erase(Q.begin());
    ProcessPoint( Q, T, L, R, I, currentPoint, intersectPoints, result );
   
  }
  return result;
}


//-----------------------------------------------------------------------------
//  Найти пересечение двух полилиний.
// ---
std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2 )
{
  std::vector<Point> intersectPoints;
  const Math::GeomPolyline* lineCurveFirst =  dynamic_cast<const Math::GeomPolyline*>( &curve1 );
  std::vector<Point> refPointsFirst = lineCurveFirst->GetReferensedPoints();
  const Math::GeomPolyline* lineCurveSecound =  dynamic_cast<const Math::GeomPolyline*>( &curve2 );
  std::vector<Point> refPointsSecound = lineCurveSecound->GetReferensedPoints();
  for (size_t i = 1; i < refPointsFirst.size(); i++)
  {
    Line firstLine(refPointsFirst[i - 1], refPointsFirst[i]);
    for (size_t j = 1; j < refPointsSecound.size(); j++)
    {
      Point point;
      if ( IntersectLines(Line(refPointsSecound[j - 1], refPointsSecound[j]), firstLine, point) )
      {
        Point currentPoint(i + point.GetX() - 1., j + point.GetY() - 1.);
        auto result = std::find(std::begin(intersectPoints), std::end(intersectPoints), currentPoint);
        if (result == std::end(intersectPoints))
          intersectPoints.push_back(currentPoint);

      }
    }
  }
  return intersectPoints;
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
std::vector<Point> IntersectLineCircle( const Curve& curve1, const Curve& curve2 )
{
  return std::vector<Point>();
}




//-----------------------------------------------------------------------------
//  Запустить метод Ньютона для поиска локального минимума квадарта расстояния между кривыми с начальной точкой в startValue.
// ---
Point NewtonMethod( const Curve& curve1, const Curve& curve2, Point startValue )
{
  size_t numiteration = 10;
  Point currentPoint = startValue;
  Point newPoint;
  for ( size_t i = 0; i < numiteration; i++ ) {
    newPoint = NewtonMethodIteration( curve1, curve2, currentPoint );
    if ( fabs(Distance(newPoint, currentPoint)) < CommonConstantsMath::NULL_TOL )
      break;
    currentPoint = newPoint;
  }
  return newPoint;
}


//-----------------------------------------------------------------------------
//  Запустить общий случай нахождения пересечения кривых.
// ---
std::vector<Point> IntersectGeneralCaseSimple( const Curve& curve1, const Curve& curve2 )
{
	GeomPolyline polylinePoints1;
  std::vector<double> refcurve1;
	curve1.GetAsPolyLine(polylinePoints1, refcurve1);

	GeomPolyline polylinePoints2;
  std::vector<double> refcurve2;
	curve2.GetAsPolyLine(polylinePoints2, refcurve2);

	std::vector<Point> startPoints = IntersectPolylinePolyline( polylinePoints1, polylinePoints2 );
 // std::cout << "Points col:" << startPoints.size() << std::endl;
	std::vector<Point> intersectPoints;

  for ( size_t i = 0; i < startPoints.size(); i++)
	{
    double paramcurve1;
    double paramcurve2;
    std::modf( startPoints[i].GetX(), &paramcurve1 );
    std::modf( startPoints[i].GetY(), &paramcurve2 );
    Point intersectPoint = NewtonMethod( curve1, curve2, Point(refcurve1[static_cast<int>(paramcurve1)], refcurve2[static_cast<int>(paramcurve2)]));
    if ( Distance( curve1.GetPoint(intersectPoint.GetX()), curve2.GetPoint(intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL )
          intersectPoints.push_back( intersectPoint );
	}
  return intersectPoints;
}


std::vector<Point> IntersectGeneralCase(const Curve& curve1, const Curve& curve2)
{
  GeomPolyline polylinePoints1;
  std::vector<double> refcurve1;
  curve1.GetAsPolyLine(polylinePoints1, refcurve1);

  GeomPolyline polylinePoints2;
  std::vector<double> refcurve2;
  curve2.GetAsPolyLine(polylinePoints2, refcurve2);

//  std::vector<Point> startPoints = IntersectPolylinePolylineBentleyOttmannAlgorithm(polylinePoints1, polylinePoints2);
  // std::cout << "Points col:" << startPoints.size() << std::endl;
  std::vector<Point> intersectPoints;

 /* for (size_t i = 0; i < startPoints.size(); i++)
  {
    double paramcurve1;
    double paramcurve2;
    std::modf(startPoints[i].GetX(), &paramcurve1);
    std::modf(startPoints[i].GetY(), &paramcurve2);
    Point intersectPoint = NewtonMethod(curve1, curve2, Point(refcurve1[static_cast<int>(paramcurve1)], refcurve2[static_cast<int>(paramcurve2)]));
    if (Distance(curve1.GetPoint(intersectPoint.GetX()), curve2.GetPoint(intersectPoint.GetY())) < CommonConstantsMath::NULL_TOL)
      intersectPoints.push_back(intersectPoint);
  }*/
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
