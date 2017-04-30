#ifndef GEOMPOLYLINE_H
#define GEOMPOLYLINE_H
#include <Math/Point.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class GeomPolyline: public GeometricPrimitive
{
private:
 std::vector<Point> referencedPoints;

public:
  //-------------------------------------------------------------------------------
  // \ru создание линии по двум точкам: началу и концу
  // ---
  GeomPolyline( const std::vector<Point>& points );
  virtual Point GetPoint            ( double t ) const; ///< возвращает точку по параметру t
  virtual Point GetDerivativePoint  ( double t ) const; ///< возвращает производную линнии по параметру t
  virtual Point Get2DerivativePoint ( double t ) const; ///< возвращает вторую производную на линии по параметру t
  virtual Range GetRange            () const;           ///< возвращает парметризацию  параметризация от [0, 1]
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< возвращает полилинию для линии (это две точки - начало и конец)
};

#endif // GEOMPOLYLINE_H
