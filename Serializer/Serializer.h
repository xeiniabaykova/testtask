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
#include "Math/Curve.h"
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

  std::unique_ptr<Math::Curve>                          Read ( std::istream& theStream );
  void                                                   Write ( std::ofstream& theStream, const Math::Curve& aCurve );
  std::vector<std::shared_ptr<Math::Curve>> ReadCurves ( std::string theFileName );
  void                                                   WriteCurves (const std::vector<std::shared_ptr<Math::Curve>>& theCurves,std::string theFileName);
private:
  std::map<std::string, std::shared_ptr<CurveSerializer>> mySerializers;

};
}
#endif // SERIALIZER_H
