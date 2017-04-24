#ifndef CIRCLECREATOR_H
#define CIRCLECREATOR_H

#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления эллипса. \~
*/
// ---
class EllipseCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает эллипс по трем точкам: одна в центре, две на границах, возвращает указатель на базовый класс для геометрии
  // ---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );
};

#endif // CIRCLECREATOR_H
