#ifndef NURBSCURVESERIALIZER_H
#define NURBSCURVESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \Класс для записи и чтения nurbs - кривой.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "../Math/NurbsCurve.h"
#include "CurveSerializer.h"


namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись  nurbs-кривой из потока.
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
  /// Чтение nurbs - кривой из потока. Данные читаются в следующем порядке: количество опорных точек, опорные точки (Point),
  /// количество узлов, узлы, количество весов, веса, степень кривой, замкнутость.
  /// В случае невалидных данных возвращается пустой геометрический примитив.
  virtual std::unique_ptr<Math::Curve> Read          ( std::istream& theInput );  ///< Чтение nurbs - кривой из потока.
  /// Запись nurbs - кривой в поток. Данные записываются в следующем порядке: количество опорных точек, опорные точки (Point),
  /// количество узлов, узлы, количество весов, веса, степень кривой, замкнутость.
  /// В случае невалидных данных в файл ничего не записывается.
  virtual void                         Write         ( std::ostream& theOutput, const Math::Curve& theCurve );
  virtual std::string                  GetHeaderName ();///< Получение заголовка примитива nurbs - кривая.
};
}
#endif
