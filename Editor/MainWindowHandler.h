#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

#include "GeometricPrimitiveCreator.h"
#include <QtCharts/QChartView>
#include <QPoint>
#include <vector>

QT_CHARTS_USE_NAMESPACE


//------------------------------------------------------------------------------
/** \brief \ru Класс обработчик событий формы mainwindow. \~
*/
// ---
class MainWindowHandler {
public:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /** \brief \ru Вспомогательные данные. \~
      \details \ru Количество точек, необходимых для отрисовки текущей кривой, обьект, отрисовывающий текущую кривую \~
    */
  // ---
  struct Creator {
    int                         sufficient;
    GeometricPrimitiveCreator * creator;
  };
private:
  std::vector<QPoint>  points;      ///< точки, полученные с экрана
  QChart             * chart;       ///< объект для отрисовки графика
  Creator              GeomCreator; ///< объект для создания геометрического примитива
public:
  MainWindowHandler (QChart * chart);

  void AddPointFromScreen           ( QPoint point ); ///< добавление точки с экрана в массив
  void AddSufficientNum             (int num);        ///< добавление необходимого количества точек
  bool IsSufficientNum              ();               ///< проверка на необходимое количество точек

  void              CreatePoint     ();               ///< создание и отображение точки
  void              CreateLine      ();               ///< создание и отображение линии
  void              CreateEllipse   ();               ///< создание и отображение Эллипса
  void              CreateNurbs     ();               ///< создание и отображение nurbs
  void              LoadFile        ();               ///< сохранение текущих кривых из файла
  void              SaveFile        ();               ///< сохранение текущих кривых в файл
  void              CreateCurve     ();               ///< общая функция для создания кривой
  void              MouseEvent( QMouseEvent *event ); ///< обработка клика мышкой

};

#endif // MAINWINDOWHANDLER_H
