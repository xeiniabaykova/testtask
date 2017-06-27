#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "Range.h"
#include "Curve.h"
namespace Math {
struct BoundingBox {
  BoundingBox( const Curve& curve );
  Range rangeX;
  Range rangeY;
  Range GetRangeX();
  Range GetRangeY();
  bool HaveIntersection( const BoundingBox& otherBox );
};

}

#endif // BOUNDINGBOX_H
