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
std::vector<Point> segmentsIntersections(const std::vector<Point>& polyline1,
  const std::vector<Point>& polyline2);

//std::vector<Point> IntersectPolylinePolylineBentleyOttmannAlgorithm( const Curve& curve1, const Curve& curve2 );
}

#endif // INTERSECTOR_H
