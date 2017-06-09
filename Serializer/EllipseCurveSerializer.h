#ifndef ELLIPSECURVESERIALIZER_H
#define ELLIPSECURVESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Класс для чтения и записи примитива эллипс.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "Math/Ellipse.h"
#include "CurveSerializer.h"
#include <ostream>

namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись эллипса.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class EllipseCurveSerializer :public CurveSerializer {
public:
  EllipseCurveSerializer() = default;
   virtual ~EllipseCurveSerializer() = default;
private:
  EllipseCurveSerializer( const EllipseCurveSerializer &obj ) = delete;
  EllipseCurveSerializer& operator=( EllipseCurveSerializer &obj ) = delete;
public:
  virtual std::unique_ptr<Math::GeometricPrimitive>Read          ( std::istream& theInput );///< Чтение эллипса из потока.
  virtual void                                     Write         ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve ); ///< Запись эллипса в поток.
  virtual std::string                              GetHeaderName ();///< Получение заголовка примитва эллпипс.
};
}
#endif
