////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  отрезок в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef LINE_H
#define LINE_H
#include "C2Curve.h"
#include "Point.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс геометрического представления линии \~
*/
// ---
class Line: public C2Curve
{
private:
  Point startPoint; ///< начальная точка
  Point endPoint;  ///< направление
public:
   Line() = delete;
   /**  \brief \ru создать отрезок по начальной и конечной точке отрезка
     \param[in] startPoint - начальная точка .\~
     \param[in] endPoint - конечная точка .\~
   */
   //---
  Line( const std::vector<Point>& points );
  Line( Point startPoint, Point endPoint );
  virtual ~Line() = default;

public:
  virtual Point        GetPoint            ( double t ) const;                                              ///< вернуть точку по параметру t
  virtual Point        GetDerivativePoint  ( double t ) const;                                              ///< вернуть производную линнии по параметру t
  virtual Point        Get2DerivativePoint ( double t ) const;                                              ///< вернуть вторую производную на линии по параметру t
  virtual Range        GetRange            () const;                                                        ///< вернуть парметризацию  параметризация от [0, 1]
  virtual void         GetAsPolyLine       ( std::vector<Point>& polyLinePoints, double accuracy ) const;  ///< вернуть полилинию для линии (это две точки - начало и конец)
          double       DistanceToPoint     ( Point point )                                          const;
  virtual void         Translate        ( double xShift, double yShift );                                ///< сдвиг по осям x, y
  virtual void         Rotate           ( double alpha );                                                ///< поворот на угол alpha
  virtual void Scale (double xScaling, double yScaling);
          std::string GetName() const;
          bool        IsValid() const;

private:
  Line( const Line &obj ) = delete;
  Line& operator=( Line &obj ) = delete;

};

#endif // LINE_H
