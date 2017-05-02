#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

#include "GeometricPrimitiveCreator.h"
#include "CurveSelector.h"
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QPoint>
#include <vector>
#include "Polyline.h"
#include "PrintFigure.h"
#include <Editor/DisplayedCurve.h>
#include <memory>


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
    int                         numExpectedPoits;
    GeometricPrimitiveCreator * creator;
    Creator( int numExpectedPoits, GeometricPrimitiveCreator * creator ):
      numExpectedPoits( numExpectedPoits ),
      creator( creator ) {}
  };
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /** \brief \ru Вспомогательные данные. \~
      \details \ru текущее состояние приложения: создание, удаление кривой, нахождение пересечения \~
    */
  // ---
  enum CurrentState
  {
    StateCreateCurve,
    StateExpectAction,
    StateDeleteCurve,
    StateFindIntersecion,
    StateCreatePolyline
  };

private:
  std::vector<Point>              points;               ///< точки, полученные с экрана
  QChart                        * chart;                ///< объект для отрисовки графика
  Creator                         geomCreator;          ///< объект для создания геометрического примитива
  PrintFigure                     printChart;           ///< объект для отображения геометрического примитива
  CurrentState                    state;                ///< объект для хранения текущего состояния окна
  CurveSelector                   selector;             ///< объект для селектирования кривых, отображаемых на экране
  std::vector<std::vector<Point>> geomPolylines;        ///< полилиния для геометрии, отображенной на экране
  std::vector<int>                isSelected;           ///< индексы селектированных кривых
  QColor                          selectingColor;       ///< цвет, обозначающий селектированную кривую
  QColor                          currentColor;         ///< цвет неселктированной кривой
  std::vector<std::shared_ptr<DisplayedCurve>>     displayedCurves;
public:
  MainWindowHandler (QChart * chart);

  void AddPointFromScreen       ( Point point );       ///< добавление точки с экрана в массив
  void AddSufficientNum         (int num);             ///< добавление необходимого количества точек
  bool IsSufficientNum          ();                    ///< проверка на необходимое количество точек

  void CreatePoint              ();                                           ///< создание и отображение точки
  void CreateLine               ();                                           ///< создание и отображение линии
  void CreateEllipse            ();                                           ///< создание и отображение эллипса
  void CreateCircle             ();                                           ///< создание и отображение окружности
  void CreateNurbs              ();                                           ///< создание и отображение nurbs
  void LoadFile                 ();                                           ///< сохранение текущих кривых из файла
  void SaveFile                 ();                                           ///< сохранение текущих кривых в файл
  void CreatePolyline           ();                                           ///< создание полилинии
  void CreateCurve              ();                                           ///< общая функция для создания кривой
  void MouseEvent               ( QMouseEvent *event );                       ///< обработка клика мышкой
  void StopCreateCurve          ();                                           ///< обработка клика мышкой
  void SetCheckableCurrentItem  ();
  void ResizeEvent              ( QResizeEvent *event );                    ///< обработка изменения размера окна
  void StateExpect              ( QMouseEvent *event );
  void ChangeColor              ( QColor color );                          ///< обработка изменения цвета кривой
  void DeleteCurve();                                                      ///< обработка удаления кривой
  void CreateEmptySeries();
  void ClearScreen();

};

#endif // MAINWINDOWHANDLER_H
