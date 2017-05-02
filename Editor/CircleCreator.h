////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель окружности в двумерном пространстве по точкам\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CIRCLECREATOR_H
#define CIRCLECREATOR_H

#include "Editor/GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru создание окружности в двумерном прострастве
  Определяется своим центром - точка в двумерном пространстве
  и радиусом (радиус всегда положителен) \~
*/
// ---
class CircleCreator: public GeometricPrimitiveCreator {
public:
  CircleCreator() = default;
  ~CircleCreator() = default;

private:
  CircleCreator( const CircleCreator &obj ) = delete;
  CircleCreator& CircleCreator::operator=( CircleCreator &obj ) = delete;

public:
  /**  \brief \ru Получить окружность по массиву из точек.
    \param[in] points - \ru Вектор из точек: points[0] - точка в центре окружности,points[1] - точка, лежащая на окружности .\~
    \return \ru Указатель на базовый класс геометрии.\~
  */
  //---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );

};

#endif // CIRCLECREATOR_H
