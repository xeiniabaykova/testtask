#ifndef CURVESERIALIZER_H
#define CURVESERIALIZER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Общий класс для записи и чтения кривых.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include <Math/GeometricPrimitive.h>
#include <Math/Point.h>
#include <istream>
#include <memory>

namespace Serializer {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение и запись геометрического примитива.
/// Чтение и запись производяться в бинарном формате.
/**
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
  virtual std::unique_ptr<Math::GeometricPrimitive> Read       ( std::istream& theInput ) = 0; ///< Чтение геометрического примитива из потока.
  /// Запись геометрического примитва в поток.
  virtual void                                      Write       ( std::ostream& theOutput, const Math::GeometricPrimitive& theCurve ) = 0;
  Math::Point                                       ReadPoint   ( std::istream& input ) const;                            ///< Чтение точки из потока.
  void                                              WritePoint  ( std::ostream& output, const Math::Point& point ) const; ///< Запись точки в поток.
  double                                            ReadDouble  ( std::istream& input ) const;                            ///< Чтение числа с плавающей запятой из потока.
  int                                               ReadInt     ( std::istream& input ) const;                            ///< Чтение целочисленного значения из потока.
  void                                              WriteInt    ( std::ostream& output, int value ) const;                ///< Запись целочисленного значения в поток.
  void                                              WriteDouble ( std::ostream& otutput, double value ) const;            ///< Запись числа с плавающей запятой в поток.
};

}
#endif // CURVESERIALIZER_H
