#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

#include "GeometricPrimitiveCreator.h"
#include <QtCharts/QChartView>
#include <QPoint>
#include <vector>
#include "DisplayedFigure.h"
#include "PrintFigure.h"



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
    Creator( int sufficient, GeometricPrimitiveCreator * creator ):
      sufficient( sufficient ),
      creator( creator ) {}
  };
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /** \brief \ru Вспомогательные данные. \~
      \details \ru текущее состояние приложения: создание, удаление кривой, нахождение пересечения \~
    */
  // ---
  enum CurrentState {
    StateCreateCurve,
    StateExpectAction,
    StateDeleteCurve,
    StateFindIntersecion
  };
private:
  std::vector<Point>           points;      ///< точки, полученные с экрана
  QChart                       * chart;     ///< объект для отрисовки графика
  Creator                      GeomCreator; ///< объект для создания геометрического примитива
  std::vector<DisplayedFigure> fugures;     ///< объект для хранения отображаемых примитивов
  PrintFigure                  printChart;  ///< объект для отображения геометрического примитива
  CurrentState                 state;       ///< объект для хранения текущего состояния окна
public:
  MainWindowHandler (QChart * chart);

  void AddPointFromScreen       ( Point point );       ///< добавление точки с экрана в массив
  void AddSufficientNum         (int num);             ///< добавление необходимого количества точек
  bool IsSufficientNum          ();                    ///< проверка на необходимое количество точек

  void CreatePoint              ();                     ///< создание и отображение точки
  void CreateLine               ();                     ///< создание и отображение линии
  void CreateEllipse            ();                     ///< создание и отображение Эллипса
  void CreateNurbs              ();                     ///< создание и отображение nurbs
  void LoadFile                 ();                     ///< сохранение текущих кривых из файла
  void SaveFile                 ();                     ///< сохранение текущих кривых в файл
  void CreateCurve              ();                     ///< общая функция для создания кривой
  void MouseEvent               ( QMouseEvent *event ); ///< обработка клика мышкой
  void StopCreateCurve          ();                     ///< обработка клика мышкой
  void PrintCharacteristicPoint ( Point point );        ///< отображение на экране точек, выбранных пользователем
  void SetCheckableCurrentItem  ();

};

#endif // MAINWINDOWHANDLER_H
