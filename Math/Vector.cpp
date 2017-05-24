#include "Vector.h"


Vector::Vector(double x1, double x2):
  x(x1),
  y(x2)
{
}
Vector::Vector(const Vector& vector)
{
  x = vector.x;
  y = vector.y;
}
double Vector::GetX() const
{
  return x;
}
double Vector::GetY() const
{
  return y;
}
