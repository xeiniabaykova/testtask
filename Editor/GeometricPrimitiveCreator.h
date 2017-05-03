////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  базовый класс - создатель геометрического примитива по массиву точек\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPRIMITIVECREATOR_H
#define GEOMETRICPRIMITIVECREATOR_H

#include <Math/GeometricPrimitive.h>
#include <memory>
#include <Math/Point.h>
#include <vector>


//------------------------------------------------------------------------------
/** \brief \ru создание геометрического объекта по массиву точек.
  на вход отдается массив точек, на выход - указатель на базовый класс геометрического примитива
  \~
*/
// ---
class GeometricPrimitiveCreator {
public:
  GeometricPrimitiveCreator() = default;
  virtual ~GeometricPrimitiveCreator() = default;

private:
  GeometricPrimitiveCreator( const GeometricPrimitiveCreator &obj ) = delete;
  GeometricPrimitiveCreator& operator=( GeometricPrimitiveCreator &obj ) = delete;

public:
  /**  \brief \ru Получить указатель на геометрический примитив по набору точек.
    \param[in] points - \ru вектор из точек, по которому вычисляется геометрический примитив
    \return \ru Указатель на базовый класс геометрии.\~
  */
  //---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points ) const = 0;

};

#endif // GEOMETRICPRIMITIVECREATOR_H
