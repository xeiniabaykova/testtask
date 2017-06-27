#ifndef INTERSECTOR_H
#define INTERSECTOR_H
#include <vector>
#include <functional>
#include "Point.h"
#include "Curve.h"
class Line;
class Ellipse;

namespace Math {
/// Нахождение всех точек пересечения кривых.
std::vector<Point> Intersector( const std::vector<Curve> curves );
/// Найти пересечение двух отрезков.
std::vector<Point> IntersectLines( const Curve& curve1, const Curve& curve2 );
/// Найти точки пересечения двух полилиний.
std::vector<Point> IntersectPolylinePolyline( const Curve& curve1, const Curve& curve2 );
/// Запустить общий алгоритм пересечения кривых.
std::vector<Point> IntersectGeneralCase( const Curve& curve1, const Curve& curve2 );


}

#endif // INTERSECTOR_H
