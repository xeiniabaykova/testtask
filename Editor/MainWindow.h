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
  QAction           * createCircleAct;     ///< \ru событие создания окружности
  QAction           * createPolylineAct;     ///< \ru событие создания окружности
  QAction           * createNurbsAct;       ///< \ru событие создания nurbs - кривой
  QAction           * stopCreateCurveAct;   ///< \ru событие прекращения создания кривых
  QAction           * reColorAct;           ///< \ru событие изменение цвета кривой
  QAction           * findIntersectionAct;  ///< \ru событие нахождения пересечения

  QAction           * deleteCurveAct;       ///< \ru событие удаления кривой

  QAction           * saveAct;              ///< \ru событие сохранения в файл
  QAction           * loadAct;              ///< \ru событие загрузки из файла

  QAction           * clearScreenAct;       ///< \ru событие очистки экрана
  MainWindowHandler windowHandler;          ///< \ru вспомогательный класс для обработки событий
  QActionGroup      * creatorCurves;        ///< \ru группа для меню создания кривых
private slots:
  /// \ru обработчик события открытия файла
  void OnSaveFile         ();
   /// \ru обработчик события сохранения файла
  void OnLoadFile         ();

  /// \ru обработчик события создания точки
  void OnCreatePoint      ();
  /// \ru обработчик события создания линии
  void OnCreateLine       ();
  /// \ru обработчик события создания эллипса
  void OnCreateEllipse    ();
  /// \ru обработчик события создания окружности
  void OnCreateCircle     ();
  /// \ru обработчик события создания nurbs
  void OnCreateNurbs      ();
   /// \ru обработчик события прекращения создания кривых
  void OnStopCreateCurve  ();
  /// \ru обработчик события нахождения пересечения
  void OnFindIntersection ();
  /// \ru обработчик события удаления кривой
  void OnDeleteCurve      ();
  /// \ru обработчик события очистки экрана
  void OnClearScreen      ();
  /// \ru обработчик события создания полилинии
  void OnCreatePolyline   ();
  /// \ru обработчик изменения цвета кривой
  void OnReColorCurve     ();
  void contextMenuRequested( const QPoint& point);
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

public:
  /** \brief \ru Инициализация.
               \param[in] chart  - \ru объект для отрисовки графика.\~
           */
  explicit MainWindow          ( QChart *chart, QWidget *parent = 0 );
  /// \ru обработчик события клика мышкой
  void mousePressEvent         ( QMouseEvent *event );
 // void MainWindow::contextMenuEvent(QContextMenuEvent *event);
 
  /// \ru обработчик события изменения размера экрана
  void resizeEvent(QResizeEvent *event);
  /// \ru создать события
  void CreateActions           ();
  /// \ru создать меню
  void CreateMenus             ();
  ~MainWindow                  ();
};

#endif // MAINWINDOW_H
