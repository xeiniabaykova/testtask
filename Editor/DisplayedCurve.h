#ifndef DISPLAYEDCURVE_H
#define DISPLAYEDCURVE_H
#include <Math/GeometricPrimitive.h>
#include <memory>

struct DisplayedCurve
{
  std::vector<Point>   referencedPoints;
  std::shared_ptr<GeometricPrimitive> primitive;
};

#endif // DISPLAYEDCURVE_H
