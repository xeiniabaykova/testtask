#ifndef ELLIPSECREATOR_H
#define ELLIPSECREATOR_H

#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления эллипса. \~
*/
// ---
class EllipseCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает эллипс по двум точкам: одна в центре, две на границах, возвращает указатель на базовый класс для геометрии
  // ---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );
};

#endif // ELLIPSECREATOR_H
