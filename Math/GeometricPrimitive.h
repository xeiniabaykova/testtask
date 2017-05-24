////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  базовый класс геометрического примитива в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include "Range.h"
#include <vector>

//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для базового  геометрического объекта
 *   \~
*/
// ---
class GeometricPrimitive { 
public:
  GeometricPrimitive() = default;
  virtual ~GeometricPrimitive  () = default;
private:
  GeometricPrimitive( const GeometricPrimitive &obj ) = delete;
  GeometricPrimitive& operator=( GeometricPrimitive &obj ) = delete;
public:
  virtual void Translate ( double xShift, double yShift ) = 0;
  virtual void Rotate    ( double alpha ) = 0;
  virtual void Scale ( double XScaling, double YScaling ) = 0;
};

#endif // GEOMETRICPRIMITIVE_H
