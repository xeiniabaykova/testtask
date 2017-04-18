#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QLabel;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
   Ui::MainWindow * ui;
   QMenu          * importExportMenu;
   QMenu          * createCurveMenu;
   QAction        * saveAct;
   QAction        * loadAct;
   QAction        * createPointAct;
   QAction        * createLineAct;
   QAction        * createCircleAct;
   QAction        * createNurbsAct;
   QLabel         * infoLabel;

private slots:
    void          SaveFile       ();
    void          LoadFile       ();
    void          CreatePoint    ();
    void          CreateLine     ();
    void          CreateCircle   ();
    void          CreateNurbs    ();

public:
    explicit MainWindow          ( QWidget *parent = 0 );
    void CreateActions           ();
    void CreateMenus             ();
    ~MainWindow                  ();
};

#endif // MAINWINDOW_H
