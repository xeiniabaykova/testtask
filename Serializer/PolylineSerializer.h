#ifndef POLYLINESERIALIZER_H
#define PLYLINESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Класс для чтения и записи примитива полилиния.\~

*/
////////////////////////////////////////////////////////////////////////////////

#include "../Math/GeomPolyline.h"
#include "CurveSerializer.h"


namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись полилинии.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class PolylineSerializer:public CurveSerializer {
public:
   PolylineSerializer() = default;
  virtual ~PolylineSerializer() = default;

private:
  PolylineSerializer( const PolylineSerializer &obj ) = delete;
  PolylineSerializer& operator=( PolylineSerializer &obj ) = delete;

public:
  /// Чтение полилинии из потока. Данные читаются в следующем порядке: количество точек, точки (Point).
  virtual std::unique_ptr<Math::Curve> Read          ( std::istream& theInput );
  /// Данные записываются в следующем порядке: количество точек, точки (Point).
  virtual void                         Write         ( std::ostream& theOutput, const Math::Curve& theCurve );
  virtual std::string                  GetHeaderName (); ///< Получение заголовка примитива отрезок.
};
}
#endif // POLYLINESERIALIZER_H
