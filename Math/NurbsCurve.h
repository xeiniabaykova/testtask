////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  nurbs - кривая в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef NURBSCURVE_H
#define NURBSCURVE_H
#include "C2Curve.h"
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
private:
  NurbsCurve( const NurbsCurve &obj ) = delete;
  NurbsCurve& NurbsCurve::operator=( NurbsCurve &obj ) = delete;
public:
  NurbsCurve() = delete;
  /**  \brief \ru создать nurbs - кривую по опорным точкам, весам, опорным точкам и замкнутости
    \param[in] points - опорные точки .\~
    \param[in] referencePoints - \ru опорные точки .\~
    \param[in] weights - \ru веса .\~
     \param[in] isClosed - \ru замкнутость .\~
  */
  //---
  NurbsCurve ( const std::vector<Point>& poles, const std::vector<double>& weights,
               const std::vector<int>& nodes, bool isClosed );

  virtual Point  GetPoint            ( double t ) const;                                             ///< возвращает точку на кривой по параметру t
  virtual Point  GetDerivativePoint  ( double t ) const;                                             ///< возвращает производную по параметру t
  virtual Point  Get2DerivativePoint ( double t ) const;                                             ///< возвращает вторую производную по параметру t
  virtual Range  GetRange            () const;                                                       ///< возвращает вторую производную по параметру t
  virtual void   GetAsPolyLine       ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< возвращает полилинию для Nurbs - кривой с точностью accuracy
         double DistancePointToCurve ( Point point ) const;
  virtual void   Translation         ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Rotation            ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Dilatation          ( const std::vector<std::vector<double>>& transformationMatrix );
  ~NurbsCurve() = default;
};


#endif // NURBSCURVE_H
