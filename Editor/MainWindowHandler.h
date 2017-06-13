////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Обработка событий главной формы\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QPoint>
#include <vector>
#include <Editor/DisplayedObject.h>
#include <Editor/CreatorHandler.h>
#include <memory>


namespace Editor {
///////////////////////////////////////////////////////////////////////////////
//
/// Класс обработчик событий формы mainwindow.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class MainWindowHandler {
  enum CurrentState
  {
    StateCreateCurve,
    StateExpectAction,
    StateFindIntersecion,
    StateCreateLine,
    StopCreatePolyline
  };

private:
  QColor                                        selectedColor;         ///< Цвет селекируемой кривой.
  QChart                                        * chart;               ///< Объект для отрисовки графика.
  std::shared_ptr<CreatorHandler>               geomCreator;           ///< Объект для создания геометрического примитива.
  CurrentState                                  state;                 ///< Объект для хранения текущего состояния окна.
  std::vector<std::shared_ptr<DisplayedObject>> displayedCurves;       ///< Набор графиков, отображенных на экране.
  QValueAxis                                    *axisX;                ///< Ось Оу.
  QValueAxis                                    *axisY;                ///< Ось Ох.
  QScatterSeries                                *tempSeriesReferenced; ///< Объект для отображения точек, выбранных пользователем.

public:
  /**  \brief \ru
    \param[in] chart - вспомогательный объект для отрисовки графика
    \return \ru Указатель на геометрическое представление полилинии.\~
  */
  explicit MainWindowHandler (QChart * chart);
  ~MainWindowHandler () = default;

private:
  MainWindowHandler( const MainWindowHandler &obj ) = delete;
  MainWindowHandler& operator=( MainWindowHandler &obj ) = delete;

public:
public:
 // void CreatePoint            ();                         ///< создать и отобразить точку
  void CreateLine             ();                         ///< Создать и отобразить отрезок.
  void CreateEllipse          ();                         ///< Создать и отобразить эллипс.
  void CreateCircle           ();                         ///< Создать и отобразить окружность.
  void CreateNurbs            ();                         ///< Создать и отобразить nurbs.
  void LoadFile               ();                         ///< Загрузить кривые из файла.
  void SaveFile               ();                          ///< Сохранить кривые в файл.
  void CreatePolyline         ();                         ///< Создать и отобразить полилинию.
  void CreateCurve            ();                         ///< Создать кривую.
  void MouseEvent             ( QMouseEvent *event );     ///< Обработать событие клика мышкой.
  void StopCreateCurve        ();                         ///< Обработать ожидание создания кривой.
  void ResizeEvent            ( QResizeEvent *event );    ///< Обработать изменение размера окна.
  void StateExpect            ( QMouseEvent *event );     ///< Обработать ожидаение клика мышкой при отсуствии создания кривых.
  void ChangeColor            ( QColor color );           ///< Обработать изменение цвета кривой.
  void DeleteCurve            ();                         ///< Обработать удаление кривой.
  void CreateEmptySeries      ();                         ///< КОСТЫЛЬ! необходим для верной работы функции map to value.
  void ClearScreen            ();                         ///< Обработать очистку экрана.
  void CreateChart            ();                          ///< Обработать создание экрана.
  void CreateRefPoint         ( Math::Point point );           ///< Отобразить точку, выбранную пользователем.
  void AddInformationNurbs    ( bool& isClosed, int& degree ); /// Вызвать меню для получения информации о нурбс - кривой.

};
}
#endif // MAINWINDOWHANDLER_H
