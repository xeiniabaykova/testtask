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
  CreatorHandler() = default;
  /**  \brief \ru получить примитив по точкам, полученным с экрана.
    \param[in] numExpectedPoits - количество точек, необходимых для создания очередного примитива
      \param[in]  creator - указатель на создателя геометрического примитива\~
  */
  //---
  CreatorHandler( int numExpectedPoits, GeometricPrimitiveCreator * creator ):
    numExpectedPoits ( numExpectedPoits ),
    creator          ( creator )
  {
  }

private:
  CreatorHandler( const CircleCreator &obj ) = delete;
  CreatorHandler& operator=( CircleCreator &obj ) = delete;

  int                         numExpectedPoits; ///< количество точек, необходимых для создания геометрического примитива
  GeometricPrimitiveCreator * creator;          ///< создатель геометрического примитива
  std::vector<Point>          points;           ///< точки, полученные с экрана

public:
  void                                AddPointFromScreen ( Point point ); ///< добавить точку с экрана в массив текущих точек
  void                                AddSufficientNum   ( int num );     ///< добавить необходимое для отрисовки кривой количество точек
  bool                                IsSufficientNum    () const;        ///< проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой
  std::shared_ptr<GeometricPrimitive> Create             ();              ///< создать геометрический примитив
  const std::vector<Point>&           RefPoints          () const;        ///< вернуть опорные точки
  void                                ClearPoints        ();              ///< очистить массив точек, полученных с экрана
};

#endif // CREATORHANDLER_H
