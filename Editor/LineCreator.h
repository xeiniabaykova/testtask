#ifndef LINECREATOR_H
#define LINECREATOR_H
#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления линии. \~
**/
// ---
class LineCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает линию по двум точкам: началу и концу
  // ---
 virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );
};

#endif // LINECREATOR_H
