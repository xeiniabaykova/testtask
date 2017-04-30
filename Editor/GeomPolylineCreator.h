#ifndef GEOMPOLYLINECREATOR_H
#define GEOMPOLYLINECREATOR_H

#include <Editor/GeometricPrimitiveCreator.h>

//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления полилинии. \~
**/
// ---
class GeomPolylineCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает линию по двум точкам: началу и концу
  // ---
 virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );
};


#endif // GEOMPOLYLINECREATOR_H
