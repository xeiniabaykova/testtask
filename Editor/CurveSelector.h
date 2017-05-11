////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  селектор полилинии по признаку близости к точке\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CURVESELECTOR_H
#define CURVESELECTOR_H

#include <Math/Point.h>
#include <vector>

double SelectedPolyline( const std::vector<Point>& polyline, Point point );

#endif // CURVESELECTOR_H

