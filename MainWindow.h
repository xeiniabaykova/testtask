#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "CollectPoints.h"
#include "GeometricPrimitiveCreator.h"

class QLabel;
namespace Ui {
class MainWindow;
}

QT_CHARTS_USE_NAMESPACE
class FileIO;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
   Ui::MainWindow * ui;

   QMenu                     * importExportMenu;
   QMenu                     * createCurveMenu;
   QMenu                     * optionsMenu;
   QMenu                     * screenMenu;

   QAction                   * createPointAct;
   QAction                   * createLineAct;
   QAction                   * createCircleAct;
   QAction                   * createNurbsAct;

   QAction                   * findIntersectionAct;

   QAction                   * deleteCurveAct;
   QLabel                    * infoLabel;

   QAction                   * saveAct;
   QAction                   * loadAct;

   QAction                   * clearScreenAct;
   QChart                    * chart;
   CollectPoints               pointsfromscreen;
   GeometricPrimitiveCreator * creatorGeometry;
private slots:

    void          SaveFile         ();
    void          LoadFile         ();


    void          CreatePoint      ();
    void          CreateLine       ();
    void          CreateEllipse    ();
    void          CreateNurbs      ();

    void          FindIntersection ();

    void          DeleteCurve      ();

    void          ClearScreen      ();

public:
    explicit MainWindow          ( QChart *chart, QWidget *parent = 0 );
    void mousePressEvent         ( QMouseEvent *event );
    void CreateActions           ();
    void CreateMenus             ();
    ~MainWindow                  ();
};

#endif // MAINWINDOW_H
