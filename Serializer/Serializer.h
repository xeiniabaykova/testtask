#ifndef SERIALIZER_H
#define SERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  Чтение и запись кривых из файла/ в файл.\~

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
///////////////////////////////////////////////////////////////////////////////
// Класс для чтения или записи набора кривых в файл/ из файла.
/**
 Каждый примитив определяется по его заголовку. После него следуют данные расположенные
 в определенном порядке для каждого примитива.
*/
///////////////////////////////////////////////////////////////////////////////
class Serializer {
public:
  Serializer();
  virtual ~Serializer() = default;

private:
  Serializer( const Serializer &obj ) = delete;
  Serializer& operator=( Serializer &obj ) = delete;

private:
  std::map<std::string, std::shared_ptr<CurveSerializer>> mySerializers;

public:
  template<typename FormattedSerializer>
  void                                      RegisterSerializer (); ///< Добавить читатели/писатели для всех типов кривых.
  /// Прочитать кривую из потока. Прочитать заголовок, затем вызывается нужный читатель для типа.
  std::unique_ptr<Math::Curve>              Read               ( std::istream& theStream ) const;
  /// Записать кривую в поток. Сначала записывается заголовок, затем вызывается нужный писатель для типа.
  void                                      Write              ( std::ofstream& theStream, const Math::Curve& aCurve ) const;
  /// Прочитать кривые из потока. Файл открывается в бинарном формате, кривые читаются в порядке нахождения в файле.
  std::vector<std::shared_ptr<Math::Curve>> ReadCurves         ( std::string theFileName ) const;
  ///Записать кривые в поток. Файл открывается в бинарном формате, кривые записываеются в порядке нахождения в curves.
  void                                      WriteCurves        ( const std::vector<std::shared_ptr<Math::Curve>>& theCurves,std::string theFileName ) const;
  /// Прочитать версию читателя - писателя.
  std::string                               ReadVersion        (  std::istream& theStream ) const;
   /// Записать версию читателя - писателя.
  void                                      WriteVersion       (  std::ofstream& stream ) const;
};
}
#endif // SERIALIZER_H
