#include "Ellipse.h"
#include <vector>

Ellipse::Ellipse(const std::vector<QPoint>& points)
{
  center.setX(5);
  center.setY(5);
  r1 = 5;
  r2 = 5;

}
QPoint Ellipse::GetPoint( double t )
{
  return QPoint (center.x() + r1 * sin(t), center.y() + r2 * cos(t) );
}

QPoint Ellipse::GetRange()
{
  return QPoint ( 0.0, 2 * 3.14 );
}
