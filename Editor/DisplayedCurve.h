////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  хранение геометрического примитива и опорных точек\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef DISPLAYEDCURVE_H
#define DISPLAYEDCURVE_H

#include <Math/GeometricPrimitive.h>
#include <memory>

//------------------------------------------------------------------------------
/** \brief \ru Хранение геометрического примитива и опорных точек:
  опорные точки - точки, на основе которых строится геометрический примитив \~
*/
// ---
struct DisplayedCurve
{
public:
  DisplayedCurve() = default;
  std::vector<Point>                  referencedPoints; ///< геометрический примитив
  std::shared_ptr<GeometricPrimitive> primitive;        ///< опорные точки
  DisplayedCurve( std::vector<Point> referencedPoints, std::shared_ptr<GeometricPrimitive> primitive ):
    referencedPoints ( referencedPoints ),
    primitive ( primitive ){}
  ~DisplayedCurve() = default;
private:
  DisplayedCurve( const DisplayedCurve &obj ) = delete;
  DisplayedCurve& DisplayedCurve::operator=( DisplayedCurve &obj ) = delete;
};

#endif // DISPLAYEDCURVE_H
