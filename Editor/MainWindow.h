////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Представление главной формы\~

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
///////////////////////////////////////////////////////////////////////////////
//
/// Класс главной формы.
/**
*/
///////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  Ui::MainWindow    * ui;                   ///< Графический интерфейс главного окна.

  QMenu             * importExportMenu;     ///< Меню записи и загрузки файла.
  QMenu             * createCurveMenu;      ///< Меню создания кривых.
  QMenu             * optionsMenu;          ///< Меню настроек.
  QMenu             * screenMenu;           ///< Меню состояния экрана.
  QMenu             * contextMenu;          ///< Контеткстное меню.

  QAction           * createPointAct;       ///< Событие создания точки.
  QAction           * createLineAct;        ///< Событие создания линии.
  QAction           * createEllipseAct;     ///< Событие создания эллипса.
  QAction           * createCircleAct;      ///< Событие создания окружности.
  QAction           * createPolylineAct;    ///< Событие создания окружности.
  QAction           * createNurbsAct;       ///< Событие создания nurbs - кривой.
  QAction           * stopCreateCurveAct;   ///< Событие прекращения создания кривых.
  QAction           * reColorAct;           ///< Событие изменение цвета кривой.
  QAction           * findIntersectionAct;  ///< Событие нахождения пересечения.

  QAction           * deleteCurveAct;       ///< Событие удаления кривой.

  QAction           * saveAct;              ///< Событие сохранения в файл.
  QAction           * loadAct;              ///< Событие загрузки из файла.

  QAction           * clearScreenAct;       ///< Событие очистки экрана.
  Editor::MainWindowHandler   windowHandler;        ///< Вспомогательный класс для обработки событий.
  QActionGroup      * creatorCurves;        ///< Группа для меню создания кривых.

public:
  /** \brief Инициализация.
               \param[in] chart  - объект для отрисовки графика.\~
           */
  explicit MainWindow ( QChart *chart, QWidget *parent = 0 );
  virtual ~MainWindow();
private:
  MainWindow( const MainWindow &obj ) = delete;
  MainWindow& operator=( MainWindow &obj ) = delete;

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

public:
  void mousePressEvent     ( QMouseEvent *event );  /// Обработать событие клика мышкой.

  void resizeEvent         (QResizeEvent *event);   /// Обработчик события изменения размера экрана.
  void CreateActions       ();                      /// Создать события.
  void CreateMenus         ();                      /// Создать меню

private slots:
  void OnSaveFile          ();                      /// Обработать событие открытия файла.
  void OnLoadFile          ();                      /// Обработать событие сохранения файла.
  void OnCreatePoint       ();                      /// Обработать событие создания точки.
  void OnCreateLine        ();                      /// Обработать событие создания линии.
  void OnCreateEllipse     ();                      /// Обработать событие создания эллипса.
   void OnCreateCircle     ();                      /// Обработать событие создания окружности
  void OnCreateNurbs       ();                      /// Обработать событие создания nurbs.
  void OnStopCreateCurve   ();                      /// Обработать событие прекращения создания кривых.
  void OnFindIntersection  ();                      /// Обработать событие нахождения пересечения.
  void OnDeleteCurve       ();                      /// Обработать событие удаления кривой.
  void OnClearScreen       ();                      /// Обработать событие очистки экрана.
  void OnCreatePolyline    ();                      /// Обработать событие создания полилинии.
  void OnReColorCurve      ();                      /// Обработать событие изменения цвета кривой.
  void contextMenuRequested( const QPoint& point ); /// Создать контекстное меню.

};

#endif // MAINWINDOW_H
