#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include "MainWindowHandler.h"

class QLabel;
namespace Ui {
class MainWindow;
}

///////////////////////////////////////////////////////////////////////////////
//
/// Класс, представляющий клавное окно
/**
*/
///////////////////////////////////////////////////////////////////////////////
///

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  Ui::MainWindow    * ui;

  QMenu             * importExportMenu;
  QMenu             * createCurveMenu;
  QMenu             * optionsMenu;
  QMenu             * screenMenu;

  QAction           * createPointAct;
  QAction           * createLineAct;
  QAction           * createCircleAct;
  QAction           * createNurbsAct;

  QAction           * findIntersectionAct;

  QAction           * deleteCurveAct;
  QLabel            * infoLabel;

  QAction           * saveAct;
  QAction           * loadAct;

  QAction           * clearScreenAct;
  MainWindowHandler windowHandler;
private slots:

  void OnSaveFile ();
  void OnLoadFile ();


  void OnCreatePoint      ();
  void OnCreateLine       ();
  void OnCreateEllipse    ();
  void OnCreateNurbs      ();

  void OnFindIntersection ();

  void OnDeleteCurve      ();

  void OnClearScreen      ();

public:
  explicit MainWindow          ( QChart *chart, QWidget *parent = 0 );
  void mousePressEvent         ( QMouseEvent *event );
  void CreateActions           ();
  void CreateMenus             ();
  ~MainWindow                  ();
};

#endif // MAINWINDOW_H
