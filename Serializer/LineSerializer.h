#ifndef LINESERIALIZER_H
#define LINESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Класс для чтения и записи примитива отрезок.\~

*/
////////////////////////////////////////////////////////////////////////////////

#include "../Math/Line.h"
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
   LineSerializer() = default;
  virtual ~LineSerializer() = default;

private:
  LineSerializer( const LineSerializer &obj ) = delete;
  LineSerializer& operator=( LineSerializer &obj ) = delete;

public:
  /// Чтение отрезка из потока. Данные читаются в следующем порядке: начало отрезка (Point), конец отрезка (Point).
  /// В случае невалидных данных возвращается пустой примитив.
  virtual std::unique_ptr<Math::Curve>              Read          ( std::istream& theInput );
  /// Запись отрезка в поток. Данные записываются в следующем порядке: начало отрезка (Point), конец отрезка (Point).
  /// В случае невалидных данных в файл ничего не записывается.
  virtual void                                      Write         ( std::ostream& theOutput, const Math::Curve& theCurve);
  virtual std::string                               GetHeaderName (); ///< Получение заголовка примитива отрезок.
};
}
#endif // LINESERIALIZER_H
