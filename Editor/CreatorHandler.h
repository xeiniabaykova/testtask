#ifndef CREATORHANDLER_H
#define CREATORHANDLER_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Создатель геометрических примитивов\~

*/
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <memory>
#include "Math/Point.h"
#include "Math/Curve.h"


namespace Editor {
///////////////////////////////////////////////////////////////////////////////
//
/// Создание геометрического примитива по точкам полученным с экрана и дополнительной информации.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class CreatorHandler
{
public:
  /// Тип создаваемой кривой
  enum TypeCurve {
    CreateLine,    ///< Создается отрезок.
    CreateEllipse, ///< Создается эллипс.
    CreateNURBS,   ///< Создается nurbs - кривая.
    CreatePolyline ///< Создается полиллиния.
  };
  TypeCurve      type; ///< Хранение типа созаваемой кривой.

private:
  std::ptrdiff_t           numExpectedPoits; ///< Количество точек, необходимых для создания геометрического примитива.
  std::vector<Math::Point> points;           ///< Точки, полученные с экрана.

public:
  /**  Получить примитив по точкам, полученным с экрана.
    \param[in] numExpectedPoits - количество точек, необходимых для создания очередного примитива.
      \param[in]  creator - указатель на создателя геометрического примитива\~
  */
  //---
  CreatorHandler( std::ptrdiff_t theNumExpectedPoits, TypeCurve theType );

private:
  CreatorHandler() = delete;
  CreatorHandler( const CreatorHandler &obj ) = delete;
  CreatorHandler& operator=( CreatorHandler &obj ) = delete;

public:
  void                            AddPointFromScreen  ( Math::Point point );           ///< Добавить точку с экрана в массив текущих точек.
  bool                            IsSufficientNum     () const;                        ///< Проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой.
  std::shared_ptr<Math::Curve>    Create              ();                              ///< Создать геометрический примитив.
  void                            ClearPoints         ();                              ///< Очистить массив точек, полученных с экрана.
  void                            AddInformationNurbs ( bool& isClosed, size_t& degree ) const; ///< Вызвать меню для получения информации о нурбс - кривой.
};
}
#endif // CREATORHANDLER_H
