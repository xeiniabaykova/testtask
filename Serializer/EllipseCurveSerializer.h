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
  /**  \brief Прочитать эллипс из потока.
    Данные читаются в следующем порядке: центр (Point), главный радиус, побочный радиус, угол наклона относительно оси ох.
  /// В случае невалидных данных возвращается пустой примитив.
  */
  //---
  virtual std::unique_ptr<Math::Curve> Read          ( std::istream& input );
  /**  \brief Записать эллипс в поток.
        Данные записываются в следующем порядке: центр (Point), главный радиус, побочный радиус, угол наклона относительно оси ох.
        В случае невалидных данных в файл ничего не записывается.
  */
  //---
  virtual void                         Write         ( std::ostream& output, const Math::Curve& curve );
  virtual std::string                  GetHeaderName ();///< Получение заголовка примитва эллпипс.
};
}
#endif
