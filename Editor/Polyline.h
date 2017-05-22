////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель полилинии по геометрической кривой\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef POLYLINE_H
#define POLYLINE_H
#include "Math/Point.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс подсчета точек для изображения кривой.
  На вход дается базовый класс для геометрического примитива,
  в методе GetPoints расчитываются точки для отображения примитива \~
*/
// ---

#include "Math/C2Curve.h"
class Polyline {
private:
  std::vector<Point>          displayedPoints;  ///<  точки для отображения примитива
  const  C2Curve *figure;                      ///<  геометрический примитив
  double                      precision;        ///<  точность для создания полилинии
public:
  Polyline() = default;
  //-------------------------------------------------------------------------------
  // \ru на вход подается геометрический примитив
  // ---
  Polyline( const C2Curve* figure, double precision );
  //-------------------------------------------------------------------------------
  // \ru расчитываются точки для отображения геометрического примитива, возвращается массив из полученных точек
  // ---
  virtual ~Polyline() = default;

private:
  Polyline( const Polyline &obj ) = delete;
  Polyline& operator= (Polyline &obj ) = delete;
private:
  double CountingStep ( double tCurrent ); /// \ru получить по текущему tCurrent шаг по кривой, удовлятворяющий хордовому отклонению  precision
public:
  std::vector<Point> GetPoints();          ///<  получить точки полилинии

};

#endif // DISPLAYEDFIGURE_H
