#ifndef INTERSECTOR_H
#define INTERSECTOR_H
#include <vector>
#include <functional>
#include "Point.h"
#include "Curve.h"
#include "Line.h"
class Ellipse;

namespace Math {
/// Нахождение всех точек пересечения кривых.
std::vector<Point> Intersector( const std::vector<Curve> curves );
/// Найти пересечение двух отрезков.
bool IntersectLines(const Curve& curve1, const Curve& curve2, Point &thePoint);
/// Найти точки пересечения двух полилиний.
std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2 );
/// Запустить общий алгоритм пересечения кривых.
std::vector<Point> IntersectGeneralCase( const Curve& curve1, const Curve& curve2 );
std::vector<Point> segmentsIntersections(std::vector<Line> segments);
enum typeEvent {
  Intersection,
  Left,
  Right,
  NonValid
};
struct PointEvent
{
  PointEvent() : point(0., 0.), otherPoint(0., 0.), numOfSegment(0) {}
  PointEvent( Point thePoint, Point theOtherPoint, size_t theNumofSegment, typeEvent theType ) :
    point(thePoint), otherPoint(theOtherPoint), numOfSegment(theNumofSegment), type( theType ) {}
  typeEvent type;
  Point point;
  Point otherPoint;
  ptrdiff_t numOfSegment;
  Line s1;
  Line s2;
};
struct SortByX
{
  inline bool operator () (const PointEvent &lhs, const PointEvent& rhs) const
  {
    if (IsEqual(lhs.point, rhs.point))
      return false;
    if (lhs.point.GetX() > rhs.point.GetX())
      return false;
    if (lhs.point.GetX() < rhs.point.GetX())
      return true;
    if (fabs(lhs.point.GetX() - rhs.point.GetX()) < CommonConstantsMath::NULL_TOL) {
      if (lhs.point.GetY() > rhs.point.GetY())
        return false;
      else
        return true;
    }
    return false;
  }

};
inline bool operator< (const PointEvent &lhs, const PointEvent& rhs)
{
  if (IsEqual(lhs.point, rhs.point))
    return false;
  if (lhs.point.GetX() > rhs.point.GetX())
    return false;
  if (lhs.point.GetX() < rhs.point.GetX())
    return true;
  if (fabs(lhs.point.GetX() - rhs.point.GetX()) < CommonConstantsMath::NULL_TOL) {
    if (lhs.point.GetY() > rhs.point.GetY())
      return false;
    else
      return true;
  }
  return false;
}
//std::vector<Point> IntersectPolylinePolylineBentleyOttmannAlgorithm( const Curve& curve1, const Curve& curve2 );
}

#endif // INTERSECTOR_H
