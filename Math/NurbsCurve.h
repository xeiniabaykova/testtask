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
#include <string>
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
  int              degree;

private:
  NurbsCurve(const NurbsCurve &obj) = delete;
  NurbsCurve& operator=(NurbsCurve &obj) = delete;
public:
  NurbsCurve() = default;
  virtual ~NurbsCurve() = default;
  /**  \brief \ru создать nurbs - кривую по опорным точкам, весам, опорным точкам и замкнутости
  \param[in] points - опорные точки .\~
  \param[in] referencePoints - \ru опорные точки .\~
  \param[in] weights - \ru веса .\~
  \param[in] isClosed - \ru замкнутость .\~
  */
  //---
  NurbsCurve( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
    const std::vector<double>& nnodes, bool iisClosed, int ddegree ):
    poles(ppoles),
    weights(wweights),
    nodes(nnodes),
    isClosed(iisClosed),
    degree(ddegree)
  {
    if ( isClosed )
    {
      for ( size_t i=0; i<degree; ++i )
      {
        weights.push_back( weights[i] );
        poles.push_back( poles[i] );
      }
    }
  }

  virtual Point  GetPoint                    ( double t ) const;                                               ///< вернуть точку на кривой по параметру t
  virtual Range  GetRange                    () const;                                                         ///< вернуть границы параметра для эллипса : [0, 2*pi]
  virtual Vector  GetDerivativePoint          ( double t ) const;                                               ///< вернуть производную на эллипсе по параметру t
  virtual Vector  Get2DerivativePoint         ( double t ) const;                                               ///< вернуть вторую производную на эллипсе по параметру t
  virtual void   GetAsPolyLine               ( std::vector<Point> & polyLinePoints, double accuracy ) const;   ///< вернуть полилинию для эллипса
  virtual double DistanceToPoint             ( Point point ) const;                                            ///< возвращает расстояние от точки до полилинии эллипса

  virtual void        Translate              ( double xShift, double yShift );
  virtual void        Rotate                 ( double alpha );
  virtual void        Scale                  ( double XScaling, double YScaling );
  std::vector<double> BasicFunctions         ( int i, double x) const;
  double              CountWeight            ( int k , double x) const;
  double              CountWeightD           ( double x )  const;
  double              CountWeightD2          ( double x )  const;
  int                 FindSpan               ( double x ) const;
  void                ComputeBasicFunction   ( double x, int i, double & result ) const;
  void               ComputeBasicFunctionD   ( double x, int i, double& result, int derivativeOrder ) const;
  std::vector<Point> GetPoles                () const { return poles; }
  std::vector<double> GetWeights             () const { return weights; }
  std::vector<double> GetNodes() const { return nodes; }
  bool IsClosed() const { return isClosed; }
  double Degree() const { return degree; }
  std::string GetName() const;
};


#endif // NURBSCURVE_H
