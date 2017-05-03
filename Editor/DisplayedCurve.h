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
  ~DisplayedCurve() = default;
  DisplayedCurve( std::vector<Point> referencedPoints, std::shared_ptr<GeometricPrimitive> primitive ):
    referencedPoints ( referencedPoints ),
    primitive        ( primitive        )
  {
  }
private:
  DisplayedCurve( const DisplayedCurve &obj ) = delete;
  DisplayedCurve& DisplayedCurve::operator=( DisplayedCurve &obj ) = delete;

private:
  std::vector<Point>                  referencedPoints; ///< геометрический примитив
  std::shared_ptr<GeometricPrimitive> primitive;        ///< опорные точки
 public:
  std::vector<Point>&                  GetReferensedPoints() ///< получить опорные точки
  {
    return referencedPoints;
  }

  std::shared_ptr<GeometricPrimitive>& GetPrimitive()        ///< получить геометрический примитив
  {
    return primitive;
  }
};

#endif // DISPLAYEDCURVE_H
