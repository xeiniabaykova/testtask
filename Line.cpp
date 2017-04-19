#include "Line.h"

Line::Line()
{

}

QPoint Line::GetRange()
{
   return QPoint(0,1);
}

QPoint Line::GetPoint( double t )
{
    return QPoint (0,0);
}
