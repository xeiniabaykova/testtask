#include "Vector.h"


Vector::Vector(double x1, double x2):
  x1(x1),
  x2(x2)
{
}
Vector::Vector(const Vector& vector)
{
  x1 = vector.GetX1();
  x2 = vector.GetX2();
}
double Vector::GetX1() const
{
  return x1;
}
double Vector::GetX2() const
{
  return x2;
}
