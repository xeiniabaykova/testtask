#ifndef GEOMETRICPRIMITIVECREATOR_H
#define GEOMETRICPRIMITIVECREATOR_H
#include <Math/GeometricPrimitive.h>
#include <memory>
#include <Math/Point.h>
#include <vector>


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для создания геометрического объекта. \~
*/
// ---
class GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает геометрический примитив по точкам
  // ---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points ) = 0;
};

#endif // GEOMETRICPRIMITIVECREATOR_H
