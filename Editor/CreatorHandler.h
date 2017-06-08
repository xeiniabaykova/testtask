////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Создатель геометрических примитивов\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CREATORHANDLER_H
#define CREATORHANDLER_H
#include <vector>
#include <memory>
#include "Math/Point.h"
#include "Math/Curve.h"
///////////////////////////////////////////////////////////////////////////////
//
/// Создание геометрического примитива по точкам полученным с экрана и дополнительной информации.
/**
*/
///////////////////////////////////////////////////////////////////////////////

namespace Editor {
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
  int                      numExpectedPoits; ///< Количество точек, необходимых для создания геометрического примитива.
  std::vector<Math::Point> points;           ///< Точки, полученные с экрана.

public:
  /**  \brief получить примитив по точкам, полученным с экрана.
    \param[in] numExpectedPoits - количество точек, необходимых для создания очередного примитива.
      \param[in]  creator - указатель на создателя геометрического примитива\~
  */
  //---
  CreatorHandler( int theNumExpectedPoits, TypeCurve theType );
private:
  CreatorHandler() = delete;
  CreatorHandler( const CreatorHandler &obj ) = delete;
  CreatorHandler& operator=( CreatorHandler &obj ) = delete;
public:
  void                            AddPointFromScreen  ( Math::Point point ); ///< Добавить точку с экрана в массив текущих точек.
  bool                            IsSufficientNum     () const;              ///< Проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой.
  std::shared_ptr<Math::Curve>    Create              ();                    ///< Создать геометрический примитив.
  void                            ClearPoints         ();                    ///< Очистить массив точек, полученных с экрана.
  void                            AddInformationNurbs ( bool& isClosed, int& degree ); ///< Вызвать меню для получения информации о нурбс - кривой.
};
}
#endif // CREATORHANDLER_H
