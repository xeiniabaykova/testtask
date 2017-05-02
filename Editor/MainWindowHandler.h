////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  обработка событий главной формы\~

*/
////////////////////////////////////////////////////////////////////////////////

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
  /**  \brief \ru
    \param[in] chart - вспомогательный объект для отрисовки графика
    \return \ru Указатель на геометрическое представление полилинии.\~
  */
  explicit MainWindowHandler (QChart * chart);
  ~MainWindowHandler () = default;

private:
  MainWindowHandler( const MainWindowHandler &obj ) = delete;
  MainWindowHandler& MainWindowHandler::operator=( MainWindowHandler &obj ) = delete;
public:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /** \brief \ru Вспомогательные данные. \~
      \details \ru Количество точек, необходимых для отрисовки текущей кривой, обьект, отрисовывающий текущую кривую \~
    */
  // ---
  struct Creator {
    int                         numExpectedPoits;
    GeometricPrimitiveCreator * creator;
    Creator( int numExpectedPoits, GeometricPrimitiveCreator * creator )
      :numExpectedPoits ( numExpectedPoits ),
      creator           ( creator ) {}
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
  std::vector<Point>                           points;               ///< точки, полученные с экрана
  QChart                                      *chart;                ///< объект для отрисовки графика
  Creator                                      geomCreator;          ///< объект для создания геометрического примитива
  PrintFigure                                  printChart;           ///< объект для отображения геометрического примитива
  CurrentState                                 state;                ///< объект для хранения текущего состояния окна
  CurveSelector                                selector;             ///< объект для селектирования кривых, отображаемых на экране
  std::vector<std::vector<Point>>              geomPolylines;        ///< полилиния для геометрии, отображенной на экране
  std::vector<int>                             selectedIndexes;      ///< индексы селектированных кривых
  QColor                                       selectedColor;        ///< цвет, обозначающий селектированную кривую
  QColor                                       normalColor;          ///< цвет неселктированной кривой
  double                                       accuracy;             ///< точность отображения создания полилинии
  std::vector<std::shared_ptr<DisplayedCurve>> displayedCurves;

public:
  void AddPointFromScreen       ( Point point );         ///< добавить точку с экрана в массив текущих точек
  void AddSufficientNum         (int num);               ///< добавить необходимое для отрисовки кривой количество точек
  bool IsSufficientNum          ();                      ///< проверить, достаточно ли текущее количество точек для создания и отображения выбранной кривой
  void CreatePoint              ();                      ///< создать и отобразить точку
  void CreateLine               ();                      ///< создать и отобразить отрезок
  void CreateEllipse            ();                      ///< создать и отобразить эллипс
  void CreateCircle             ();                      ///< создать и отобразить окружность
  void CreateNurbs              ();                      ///< создать и отобразить nurbs
  void LoadFile                 ();                      ///< загрузить кривые из файла
  void SaveFile                 ();                      ///< сохранить кривые в файл
  void CreatePolyline           ();                      ///< создать и отобразить полилинию
  void CreateCurve              ();                      ///< создать кривую
  void MouseEvent               ( QMouseEvent *event );  ///< обработать событие клика мышкой
  void StopCreateCurve          ();                      ///< обработать ожидание создания кривой
  void ResizeEvent              ( QResizeEvent *event ); ///< обработать изменение размера окна
  void StateExpect              ( QMouseEvent *event );  ///< обработать ожидаение клика мышкой при отсуствии создания кривых
  void ChangeColor              ( QColor color );        ///< обработать изменение цвета кривой
  void DeleteCurve              ();                      ///< обработать удаление кривой
  void CreateEmptySeries        ();                      ///< КОСТЫЛЬ! необходим для верной работы функции map to value
  void ClearScreen              ();                      ///< обработать очистку экрана
  void DoubleSelectionRemoved( int indexSelectedCurve ); ///< обработать случай повтоного клика на кривую

};

#endif // MAINWINDOWHANDLER_H
