////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель геометрического представления полилинии по набору точек\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMPOLYLINECREATOR_H
#define GEOMPOLYLINECREATOR_H

#include <Editor/GeometricPrimitiveCreator.h>

//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления полилинии. \~
**/
// ---
class GeomPolylineCreator: public GeometricPrimitiveCreator {
public:
  GeomPolylineCreator() = default;
 ~GeomPolylineCreator() = default;

private:
  GeomPolylineCreator( const GeomPolylineCreator &obj ) = delete;
  GeomPolylineCreator& GeomPolylineCreator::operator=( GeomPolylineCreator &obj ) = delete;

public:
  /**  \brief \ru Получить указатель на геометрическое представление полилинии по набору точек
    \param[in] points - \ru вектор из точек, по которому вычисляется полилиния
    \return \ru Указатель на геометрическое представление полилинии.\~
  */
   virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );

};


#endif // GEOMPOLYLINECREATOR_H
