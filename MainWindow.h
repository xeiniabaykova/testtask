#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QLabel;
namespace Ui {
class MainWindow;
}

class FileIO;
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
   Ui::MainWindow * ui;

   QMenu          * importExportMenu;
   QMenu          * createCurveMenu;
   QMenu          * optionsMenu;
   QMenu          * screenMenu;

   QAction        * createPointAct;
   QAction        * createLineAct;
   QAction        * createCircleAct;
   QAction        * createNurbsAct;

   QAction        * findIntersectionAct;

   QAction        * deleteCurveAct;
   QLabel         * infoLabel;

   QAction        * saveAct;
   QAction        * loadAct;

   QAction        * clearScreenAct;

private slots:

    void          SaveFile         ();
    void          LoadFile         ();


    void          CreatePoint      ();
    void          CreateLine       ();
    void          CreateCircle     ();
    void          CreateNurbs      ();

    void          FindIntersection ();

    void          DeleteCurve      ();

    void          ClearScreen      ();

public:
    explicit MainWindow          ( QWidget *parent = 0 );
    void CreateActions           ();
    void CreateMenus             ();
    ~MainWindow                  ();
};

#endif // MAINWINDOW_H
