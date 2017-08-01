#ifndef CURVESERIALIZER_H
#define CURVESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Общий класс для записи и чтения кривых.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "GeometricPrimitive.h"
#include "Curve.h"
#include "Point.h"
#include <istream>
#include <memory>


namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись геометрического примитива.
/**
  Чтение и запись производятся в бинарном формате.
*/
///////////////////////////////////////////////////////////////////////////////
class CurveSerializer {
public:
  virtual ~CurveSerializer() = default;
    CurveSerializer() = default;

private:
  CurveSerializer( const CurveSerializer &obj ) = delete;
  CurveSerializer& operator=( CurveSerializer &obj ) = delete;

public:
  virtual std::unique_ptr<Math::Curve> Read        ( std::istream& theInput ) = 0; ///< Прочитать геометрический примитив из потока.
  /// Записать геометрический примитив в поток.
  virtual void                         Write       ( std::ostream& theOutput, const Math::Curve& theCurve ) = 0;
  Math::Point                          ReadPoint   ( std::istream& input ) const;                            ///< Прочитать точку из потока.
  void                                 WritePoint  ( std::ostream& output, const Math::Point& point ) const; ///< Записать точку в поток.
  double                               ReadDouble  ( std::istream& input ) const;                            ///< Прочитать число с плавающей запятой из потока.
  size_t                               ReadSize_t  ( std::istream& input ) const;                            ///< Прочитать целочисленное значение из потока.
  void                                 WriteSize_t ( std::ostream& output, size_t value ) const;                ///< Записать целочисленное значение в поток.
  void                                 WriteDouble ( std::ostream& otutput, double value ) const;            ///< Записать число с плавающей запятой в поток.
};

}
#endif // CURVESERIALIZER_H
