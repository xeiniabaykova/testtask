#ifndef DISPLAYEDFIGURE_H
#define DISPLAYEDFIGURE_H
#include "Math/Point.h"
#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс подсчета точек для изображения кривой.
 * На вход дается базовый класс для геометрического примитива,
 * в методе GetPoints расчитываются точки для отображения примитива \~
*/
// ---

class DisplayedFigure {

private:
  std::vector<Point>                  displayedPoints;  ///<  точки для отображения примитива
  std::shared_ptr<GeometricPrimitive> figure;           ///<  геометрический примитив
public:
  //-------------------------------------------------------------------------------
  // \ru на вход подается геометрический примитив
  // ---
  DisplayedFigure( std::shared_ptr<GeometricPrimitive> figure );
  //-------------------------------------------------------------------------------
  // \ru расчитываются точки для отображения геометрического примитива, возвращается массив из полученных точек
  // ---
  std::vector<Point>                  GetPoints();
  std::shared_ptr<GeometricPrimitive> GetFigure(); ///< возвразается геометрический примитив
};

#endif // DISPLAYEDFIGURE_H
