////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  полилиния в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMPOLYLINE_H
#define GEOMPOLYLINE_H
#include "C2Curve.h"
#include <Math/Point.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class GeomPolyline: public C2Curve
{
private:
 std::vector<Point> referencedPoints;
public:
 /**  \brief \ru создать полилинию по опорным точкам
   \param[in] points - опорные точки .\~
 */
 //---
  GeomPolyline( const std::vector<Point>& points );
  GeomPolyline() = delete;
  virtual ~GeomPolyline() = default;
private:
  GeomPolyline( const GeomPolyline &obj ) = delete;
  GeomPolyline& operator=( GeomPolyline &obj ) = delete;

public:
  virtual Point GetPoint            ( double t ) const;                                             ///< вернуть точку по параметру t
  virtual Point GetDerivativePoint  ( double t ) const;                                             ///< вернуть производную линнии по параметру t
  virtual Point Get2DerivativePoint ( double t ) const;                                             ///< вернуть вторую производную на линии по параметру t
  virtual Range GetRange            () const;                                                       ///< вернуть парметризацию  параметризация от [0, 1]
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< вернуть полилинию для полилинии (это и есть сама полилиния)
  virtual double DistancePointToCurve        ( Point point ) const;
  virtual void   Translation                 ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Rotation                    ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Dilatation                  ( const std::vector<std::vector<double>>& transformationMatrix );
};

#endif // GEOMPOLYLINE_H
