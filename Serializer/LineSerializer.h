#ifndef LINESERIALIZER_H
#define LINESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Класс для чтения и записи примитива отрезок.\~

*/
////////////////////////////////////////////////////////////////////////////////

#include <Math/Line.h>
#include "CurveSerializer.h"

namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись отрезка.
/**
*/
///////////////////////////////////////////////////////////////////////////////

class LineSerializer:public CurveSerializer {
public:
  virtual ~LineSerializer() = default;
private:
  LineSerializer() = default;
  LineSerializer( const LineSerializer &obj ) = delete;
  LineSerializer& operator=( LineSerializer &obj ) = delete;
public:
  virtual std::unique_ptr<Math::GeometricPrimitive> Read          ( std::istream& theInput );///< Чтение отрезка из потока.
  /// Запись отрезка в поток.
  virtual void                                      Write         ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve);
  virtual std::string                               GetHeaderName (); ///< Получение заголовка примитива отрезок.
};
}
#endif // LINESERIALIZER_H
