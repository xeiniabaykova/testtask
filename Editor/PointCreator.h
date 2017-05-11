////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  возможно, больше не нужно?\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef POINTCREATOR_H
#define POINTCREATOR_H

#include "GeometricPrimitiveCreator.h"
#include <Math/Point.h>


//------------------------------------------------------------------------------
/** \brief \ru Класс создания точки. \~
*/
// ---
class PointCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает геометрическое представление точки
  // ---
  PointCreator() = default;
  ~PointCreator() = default;
private:
  PointCreator( const PointCreator &obj ) = default;
  PointCreator& operator=( PointCreator &obj ) = default;
public:
  std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points ) const;

};

#endif // POINTCREATOR_H
