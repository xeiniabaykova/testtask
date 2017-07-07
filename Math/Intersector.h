#ifndef INTERSECTOR_H
#define INTERSECTOR_H
#include <vector>
#include <functional>
#include "Point.h"
#include "Curve.h"
#include "Line.h"

namespace Math {
/// Нахождение всех точек пересечения кривых.
std::vector<Point> Intersector               ( const std::vector<Curve> curves );
/// Найти пересечение двух отрезков.
bool IntersectLines                          (const Curve& curve1, const Curve& curve2, Point &thePoint);
/// Найти точки пересечения двух полилиний.
std::vector<Point> IntersectPolylinePolyline ( const Curve& curve1, const Curve& curve2, std::vector<std::pair<double, double>>& resultParams );
std::vector<Point> IntersectPolylinePolyline ( const Curve& curve1, const Curve& curve2 );
/// Запустить общий алгоритм пересечения кривых.
std::vector<Point> IntersectGeneralCase      ( const Curve& curve1, const Curve& curve2 );
std::vector<Point> SegmentsIntersections     ( std::vector<Line> segments, std::vector<std::pair<double, double>>& params);
std::vector<Point> SegmentsIntersections     ( const Math::GeomPolyline* polyline1,const Math::GeomPolyline* polyline2,
                                               std::vector<std::pair<double, double>>& params );
/// Найти точки пересечения для отрезка и окружности.
std::vector<Point> IntersectLineCircle       ( const Curve& line, const Curve& circle );

}

#endif // INTERSECTOR_H
