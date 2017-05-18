////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru  nurbs - кривая в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef NURBSCURVE_H
#define NURBSCURVE_H
#include "C2Curve.h"
#include "Point.h"
#include "Range.h"
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления Nurbs - кривой \~
*/
// ---


class NurbsCurve : public GeometricPrimitive
{
private:
  std::vector<Point>  poles;
  std::vector<double> weights;
  std::vector<double> nodes;
  bool                isClosed;
  double              degree;

private:
  NurbsCurve(const NurbsCurve &obj) = delete;
  NurbsCurve& operator=(NurbsCurve &obj) = delete;
public:
  NurbsCurve() = delete;
  virtual ~NurbsCurve() = default;
  /**  \brief \ru создать nurbs - кривую по опорным точкам, весам, опорным точкам и замкнутости
  \param[in] points - опорные точки .\~
  \param[in] referencePoints - \ru опорные точки .\~
  \param[in] weights - \ru веса .\~
  \param[in] isClosed - \ru замкнутость .\~
  */
  //---
  NurbsCurve( const std::vector<Point>& poles, const std::vector<double>& weights,
    const std::vector<double>& nodes, bool isClosed, int degree ):
    poles(poles),
    weights(weights),
    nodes(nodes),
    isClosed(isClosed),
    degree(degree)
  {
  }

  virtual Point  GetPoint               ( double t ) const;                                             ///< возвращает точку на кривой по параметру t
  virtual Point  GetDerivativePoint     ( double t ) const;                                             ///< возвращает производную по параметру t
  virtual Point  Get2DerivativePoint    ( double t ) const;                                             ///< возвращает вторую производную по параметру t
  virtual Range  GetRange               ()         const;                                                       ///< возвращает вторую производную по параметру t
  virtual void   GetAsPolyLine          ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< возвращает полилинию для Nurbs - кривой с точностью accuracy
  double         DistancePointToCurve   ( Point point ) const;
  virtual void   Translation            ( double xShift, double yShift );
  virtual void   Rotation               ( double alpha );
  virtual void   Dilatation             ( double XScaling, double YScaling );

  double         CountWeight            ( double x ) const;
  double         FindSpan               ( double x ) const;
  void           ComputeBasicFunction   ( double x, int i, double & result ) const;
  void           ComputeBasicFunctionsD ( double X, const std::vector<std::vector<double>>& basicFunctions, std::vector<std::vector<double>>& basicFunctionsD );
  double         BasicFunction          ();
};


#endif // NURBSCURVE_H
