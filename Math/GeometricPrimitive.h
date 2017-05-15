////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  базовый класс геометрического примитива в двумерном пространстве\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H

#include <Math/Point.h>
#include <Math/Range.h>
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
  virtual void Translation ( double xShift, double yShift ) = 0;
  virtual void Rotation    ( double alpha ) = 0;
  virtual void Dilatation  ( double XScaling, double YScaling ) = 0;
};

#endif // GEOMETRICPRIMITIVE_H
