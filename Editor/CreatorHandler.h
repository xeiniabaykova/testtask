////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель окружности в двумерном пространстве по точкам\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CREATORHANDLER_H
#define CREATORHANDLER_H
#include <Editor/GeometricPrimitiveCreator.h>

class CreatorHandler
{
public:
  enum TypeCurve {
    CreateLine,
    CreateEllipse,
    CreateNURBS,
    CreatePolyline
  };
  TypeCurve      type;
private:
  int                                          numExpectedPoits; ///< количество точек, необходимых для создания геометрического примитива
  std::vector<Point>                           points;           ///< точки, полученные с экрана

public:
  CreatorHandler() = default;
  /**  \brief \ru получить примитив по точкам, полученным с экрана.
    \param[in] numExpectedPoits - количество точек, необходимых для создания очередного примитива
      \param[in]  creator - указатель на создателя геометрического примитива\~
  */
  //---
  CreatorHandler( int numExpectedPoits ):
    numExpectedPoits ( numExpectedPoits )
  {
  }

private:
  CreatorHandler( const CreatorHandler &obj ) = delete;
  CreatorHandler& operator=( CreatorHandler &obj ) = delete;
public:
  void                      AddPointFromScreen ( Point point ); ///< добавить точку с экрана в массив текущих точек
  void                      AddSufficientNum   ( int num );     ///< добавить необходимое для отрисовки кривой количество точек
  bool                      IsSufficientNum    () const;        ///< проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой
  std::shared_ptr<C2Curve>  Create             ();              ///< создать геометрический примитив
  const std::vector<Point>& RefPoints          () const;        ///< вернуть опорные точки
  void                      ClearPoints        ();              ///< очистить массив точек, полученных с экрана
};

#endif // CREATORHANDLER_H
