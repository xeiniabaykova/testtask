#ifndef CIRCLECREATOR_H
#define CIRCLECREATOR_H

#include "Editor/GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления окружности. \~
*/
// ---
class CircleCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает окружность по двум точкам: одна в центре, одна на границе, возвращает указатель на базовый класс для геометрии
  // ---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );
};

#endif // CIRCLECREATOR_H
