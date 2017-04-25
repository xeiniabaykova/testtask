#ifndef NURBSCURVE_H
#define NURBSCURVE_H

#include <Math/Point.h>
#include <Math/Range.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления Nurbs - кривой \~
*/
// ---


class NurbsCurve: public GeometricPrimitive
{
private:
  std::vector<Point>  poles;
  std::vector<double> weights;
  std::vector<int>    nodes;
  bool                isClosed;
public:
  //-------------------------------------------------------------------------------
  // \ru создание Nurbs - кривой по опорным точкам, весам, узлам и замкнутости
  // ---
  NurbsCurve (const std::vector<Point>& poles,
              const std::vector<double>& weights,
              const std::vector<int>& nodes,
              bool isClosed);

  virtual Point GetPoint            ( double t ) const;                                             ///< возвращает точку на кривой по параметру t
  virtual Range GetRange            () const;                                                       ///< возвращает границы параметра
  virtual Point GetDerivativePoint  ( double t ) const;                                             ///< возвращает производную по параметру t
  virtual Point Get2DerivativePoint ( double t ) const;                                             ///< возвращает вторую производную по параметру t
  virtual void  GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< возвращает полилинию для Nurbs - кривой с точностью accuracy

};


#endif // NURBSCURVE_H
