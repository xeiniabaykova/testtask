////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель геометрических примитивов\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CREATORHANDLER_H
#define CREATORHANDLER_H
#include <vector>
#include <memory>
#include "Math/Point.h"
#include "Math/Curve.h"
//------------------------------------------------------------------------------
/** \brief
 * Создание геометрического примитива по точкам полученным с экрана и дополнительной информации\~
*/
// ---
namespace Editor {
class CreatorHandler
{
public:
  /// Тип создаваемой кривой
  enum TypeCurve {
    CreateLine,    ///< создается отрезок
    CreateEllipse, ///< создается эллипс
    CreateNURBS,   ///< создается nurbs - кривая
    CreatePolyline ///< создается полиллиния
  };
  TypeCurve      type; ///< хранение типа созаваемой кривой
private:
  int                      numExpectedPoits; ///< количество точек, необходимых для создания геометрического примитива
  std::vector<Math::Point> points;           ///< точки, полученные с экрана

public:
  /**  \brief получить примитив по точкам, полученным с экрана.
    \param[in] numExpectedPoits - количество точек, необходимых для создания очередного примитива
      \param[in]  creator - указатель на создателя геометрического примитива\~
  */
  //---
  CreatorHandler( int theNumExpectedPoits, TypeCurve theType );
private:
  CreatorHandler() = delete;
  CreatorHandler( const CreatorHandler &obj ) = delete;
  CreatorHandler& operator=( CreatorHandler &obj ) = delete;
public:
  void                            AddPointFromScreen ( Math::Point point ); ///< добавить точку с экрана в массив текущих точек
  bool                            IsSufficientNum    () const;        ///< проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой
  std::shared_ptr<Math::Curve>    Create             ();              ///< создать геометрический примитив
  void                            ClearPoints        ();              ///< очистить массив точек, полученных с экрана
};
}
#endif // CREATORHANDLER_H
