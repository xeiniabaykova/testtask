#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Базовый класс геометрического примитива в двумерном пространстве.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "Range.h"
#include <vector>
#include <string>


namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
/// Абстрактный класс для базового  геометрического объекта.
/**
  Представлет функции для хранения, масштабирования, поворота и сдвига геометрического объекта, проверки его на правильность.
*/
///////////////////////////////////////////////////////////////////////////////
class GeometricPrimitive { 
public:
  GeometricPrimitive() = default;
  virtual ~GeometricPrimitive  () = default;

private:
  GeometricPrimitive( const GeometricPrimitive &obj ) = delete;
  GeometricPrimitive& operator=( GeometricPrimitive &obj ) = delete;

public:
  virtual void        Translate ( double xShift, double yShift ) = 0;    ///< Сдвинуть по оси x на xShift, по оси y на yShift.
  virtual void        Rotate    ( double alpha ) = 0;                    ///< Повернуть относительно начала координат на угол alpha.
  virtual void        Scale     ( double XScaling, double YScaling ) = 0;///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  virtual bool        IsValid   () const = 0;                            ///< Проверить на правильность геометрический примитив.
  virtual std::string GetName   () const = 0;                            ///< Вернуть имя, используемое при записи отрезка в файл.
};
}
#endif // GEOMETRICPRIMITIVE_H
