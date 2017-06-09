#ifndef SERIALIZER_H
#define SERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Класс для чтения или записи набора кривых в файл/ из файла.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "CurveSerializer.h"
#include "Math/GeometricPrimitive.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
namespace Serializer {
class Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/**
 Каждый примитив определяется по его заголовку. После него следуют данные расположенные
 в определенном порядке для каждого примитива.
*/
///////////////////////////////////////////////////////////////////////////////
public:
  Serializer();
  virtual ~Serializer() = default;
private:
  Serializer( const Serializer &obj ) = delete;
  Serializer& operator=( Serializer &obj ) = delete;
public:
  template<typename FormattedSerializer>
  void RegisterSerializer();

  std::unique_ptr<Math::GeometricPrimitive>              Read ( std::istream& theStream );
  void                                                   Write ( std::ostream& theStream, const Math::GeometricPrimitive& aCurve );
  std::vector<std::shared_ptr<Math::GeometricPrimitive>> ReadCurves ( std::string theFileName );
  void                                                   WriteCurves (const std::vector<std::shared_ptr<Math::GeometricPrimitive>>& theCurves,std::string theFileName);
private:
  std::map<std::string, std::shared_ptr<CurveSerializer>> mySerializers;

};
}
#endif // SERIALIZER_H
