#ifndef NURBSCURVESERIALIZER_H
#define NURBSCURVESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Класс для записи и чтения nurbs - кривой.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include <Math/NurbsCurve.h>
#include "CurveSerializer.h"

namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись геометрического nurbs -кривой.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class NURBSCurveSerializer :public CurveSerializer {

public:
  NURBSCurveSerializer() = default;
  virtual ~NURBSCurveSerializer() = default;
private:
  NURBSCurveSerializer( const NURBSCurveSerializer &obj ) = delete;
  NURBSCurveSerializer& operator=( NURBSCurveSerializer &obj ) = delete;
public:
  virtual std::unique_ptr<Math::GeometricPrimitive> Read          ( std::istream& theInput );  ///< Чтение nurbs - кривой из потока.
  /// Запись nurbs - кривой в поток.
  virtual void                                      Write         ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve );
  virtual std::string                               GetHeaderName ();///< Получение заголовка примитива nurbs - кривая.
};
}
#endif
