////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  отрезок в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef LINE_H
#define LINE_H
#include "C2Curve.h"
#include <Math/Point.h>
#include <vector>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class Line: public C2Curve
{
private:
  Point startPoint; ///< начальная точка
  Point direction;  ///< направление
public:
   Line() = delete;
   /**  \brief \ru создать отрезок по начальной и конечной точке отрезка
     \param[in] startPoint - начальная точка .\~
     \param[in] endPoint - конечная точка .\~
   */
   //---
  Line( Point startPoint, Point endPoint );
  ~Line() = default;
private:
  Line( const Line &obj ) = delete;
  Line& operator=( Line &obj ) = delete;
public:
  virtual Point  GetPoint                ( double t ) const;                                             ///< вернуть точку по параметру t
  virtual Point  GetDerivativePoint      ( double t ) const;                                             ///< вернуть производную линнии по параметру t
  virtual Point  Get2DerivativePoint     ( double t ) const;                                             ///< вернуть вторую производную на линии по параметру t
  virtual Range  GetRange                () const;                                                       ///< вернуть парметризацию  параметризация от [0, 1]
  virtual void   GetAsPolyLine           ( std::vector<Point> & polyLinePoints, double accuracy ) const; ///< вернуть полилинию для линии (это две точки - начало и конец)
  virtual double DistanceToCurvePolyline ( Point point) const;
  virtual void   Translation             ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Rotation                ( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Dilatation              ( const std::vector<std::vector<double>>& transformationMatrix );

};

#endif // LINE_H
