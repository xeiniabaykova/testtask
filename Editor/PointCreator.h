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
class PointCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает геометрическое представление точки
  // ---
  PointCreator() = default;
  ~PointCreator() = default;
private:
  PointCreator( const PointCreator &obj ) = default;
  PointCreator& PointCreator::operator=( PointCreator &obj ) = default;
public:
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points );

};

#endif // POINTCREATOR_H
