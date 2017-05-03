////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель отрезка прямой по массиву точек\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef LINECREATOR_H
#define LINECREATOR_H
#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления линии. \~
**/
// ---
class LineCreator: public GeometricPrimitiveCreator {
public:
  LineCreator() = default;
  ~LineCreator() = default;

private:
  LineCreator( const LineCreator &obj ) = delete;
  LineCreator& LineCreator::operator=( LineCreator &obj ) = delete;

public:
  /**  \brief \ru Получить отрезок прямой по массиву из точек.
   \param[in] points - \ru Вектор из точек: points[0] - точка начала отрезка, points[1] - точка конца отрезка .\~
   \return \ru Указатель на геометрическое представление отрезка геометрии.\~
  */
  //---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );

};

#endif // LINECREATOR_H
