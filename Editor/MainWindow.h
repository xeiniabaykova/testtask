////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  представление главной формы\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QActionGroup>
#include "MainWindowHandler.h"

class QLabel;
namespace Ui {
class MainWindow;
}

//------------------------------------------------------------------------------
/** \brief \ru Класс главной формы. \~
*/
// ---

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /** \brief \ru Инициализация.
               \param[in] chart  - \ru объект для отрисовки графика.\~
           */
  explicit MainWindow ( QChart *chart, QWidget *parent = 0 );
  ~MainWindow();
private:
  MainWindow( const MainWindow &obj ) = delete;
  MainWindow& MainWindow::operator=( MainWindow &obj ) = delete;

private:
  Ui::MainWindow    * ui;                   ///< \ru графический интерфейс главного окна

  QMenu             * importExportMenu;     ///< \ru меню записи и загрузки файла
  QMenu             * createCurveMenu;      ///< \ru меню создания кривых
  QMenu             * optionsMenu;          ///< \ru меню настроек
  QMenu             * screenMenu;
  QMenu             * contextMenu;

  QAction           * createPointAct;       ///< \ru событие создания точки
  QAction           * createLineAct;        ///< \ru событие создания линии
  QAction           * createEllipseAct;     ///< \ru событие создания эллипса
  QAction           * createCircleAct;      ///< \ru событие создания окружности
  QAction           * createPolylineAct;    ///< \ru событие создания окружности
  QAction           * createNurbsAct;       ///< \ru событие создания nurbs - кривой
  QAction           * stopCreateCurveAct;   ///< \ru событие прекращения создания кривых
  QAction           * reColorAct;           ///< \ru событие изменение цвета кривой
  QAction           * findIntersectionAct;  ///< \ru событие нахождения пересечения

  QAction           * deleteCurveAct;       ///< \ru событие удаления кривой

  QAction           * saveAct;              ///< \ru событие сохранения в файл
  QAction           * loadAct;              ///< \ru событие загрузки из файла

  QAction           * clearScreenAct;       ///< \ru событие очистки экрана
  MainWindowHandler   windowHandler;        ///< \ru вспомогательный класс для обработки событий
  QActionGroup      * creatorCurves;        ///< \ru группа для меню создания кривых

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

public:
  void mousePressEvent    ( QMouseEvent *event );  /// \ru обработать событие клика мышкой
  void resizeEvent        (QResizeEvent *event);   /// \ru обработчик события изменения размера экрана
  void CreateActions      ();                      /// \ru создать события
  void CreateMenus        ();                      /// \ru создать меню

private slots:
  void OnSaveFile          ();                      /// \ru обработать событие открытия файла
  void OnLoadFile          ();                      /// \ru обработать событие сохранения файла
  void OnCreatePoint       ();                      /// \ru обработать событие создания точки
  void OnCreateLine        ();                      /// \ru обработать событие создания линии
  void OnCreateEllipse     ();                      /// \ru обработать событие создания эллипса
  void OnCreateCircle      ();                      /// \ru обработать событие создания окружности
  void OnCreateNurbs       ();                      /// \ru обработать событие создания nurbs
  void OnStopCreateCurve   ();                      /// \ru обработать событие прекращения создания кривых
  void OnFindIntersection  ();                      /// \ru обработать событие нахождения пересечения
  void OnDeleteCurve       ();                      /// \ru обработать событие удаления кривой
  void OnClearScreen       ();                      /// \ru обработать событие очистки экрана
  void OnCreatePolyline    ();                      /// \ru обработать событие создания полилинии
  void OnReColorCurve      ();                      /// \ru обработать событие изменения цвета кривой
  void contextMenuRequested( const QPoint& point ); /// \ru создать контекстное меню

};

#endif // MAINWINDOW_H
