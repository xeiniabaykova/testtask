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


class NurbsCurve : public C2Curve
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

  virtual Point  GetPoint                    ( double t ) const;                                               ///< вернуть точку на кривой по параметру t
  virtual Range  GetRange                    () const;                                                         ///< вернуть границы параметра для эллипса : [0, 2*pi]
  virtual Point  GetDerivativePoint          ( double t ) const;                                               ///< вернуть производную на эллипсе по параметру t
  virtual Point  Get2DerivativePoint         ( double t ) const;                                               ///< вернуть вторую производную на эллипсе по параметру t
  virtual void   GetAsPolyLine               ( std::vector<Point> & polyLinePoints, double accuracy ) const;   ///< вернуть полилинию для эллипса
  virtual double DistanceToPoint              ( Point point ) const;                                            ///< возвращает расстояние от точки до полилинии эллипса

  virtual void Translation ( double xShift, double yShift );
  virtual void Rotation    ( double alpha );
  virtual void Scaling  ( double XScaling, double YScaling );
private:
  std::vector<double> BasicFunctions( int i, double x) const;
  double         CountWeight            ( double x ) const;
  double         CountWeightD           ( double x )  const;
  double         CountWeightD2          ( double x )  const;
  int            FindSpan               ( double x ) const;
  void           ComputeBasicFunction   ( double x, int i, double & result ) const;
  void           ComputeBasicFunctionsD ( double X, const std::vector<std::vector<double>>& basicFunctions, std::vector<std::vector<double>>& basicFunctionsD );
  void           ComputeBasicFunctionD  ( double x, int i, double& result, int derivativeOrder ) const;
  double         BasicFunction          ();
};


#endif // NURBSCURVE_H
