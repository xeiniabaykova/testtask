#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FileIO.h"
#include <QLabel>
#include "PointCreator.h"
#include "EllipseCreator.h"
#include "LineCreator.h"
#include <QtWidgets/QMainWindow>
#include <functional>
#include <cmath>
#define _USE_MATH_DEFINES

MainWindow::MainWindow( QChart *chart, QWidget *parent ) :
    chart( chart ),
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMenus()
{
    importExportMenu = menuBar()->addMenu( tr("&File") );
    importExportMenu->addAction( saveAct );
    importExportMenu->addAction( loadAct );

    createCurveMenu = menuBar()->addMenu( tr("&Curves") );
    createCurveMenu->addAction( createPointAct );
    createCurveMenu->addAction( createLineAct );
    createCurveMenu->addAction( createCircleAct );
    createCurveMenu->addAction( createNurbsAct );

    optionsMenu = menuBar()->addMenu( tr("&Options") );
    optionsMenu->addAction( deleteCurveAct );
    optionsMenu->addAction( findIntersectionAct) ;

    screenMenu = menuBar()->addMenu( tr("&Screen") );
    screenMenu->addAction( clearScreenAct );
}

void MainWindow::CreateActions()
{
    saveAct = new QAction( tr("&Save"), this );
    saveAct->setShortcuts( QKeySequence::New );
    saveAct->setStatusTip( tr("Save into file") );
    connect( saveAct, &QAction::triggered, this, &MainWindow::SaveFile );

    loadAct = new QAction ( tr("&Load"), this);
    loadAct->setShortcuts( QKeySequence::New );
    loadAct->setStatusTip( tr("Load from file") );
    connect( loadAct, &QAction::triggered, this, &MainWindow::LoadFile );

    createPointAct = new QAction ( tr("&CreatePoint"), this );
    createPointAct->setShortcuts( QKeySequence::New );
    createPointAct->setStatusTip( tr("Creating point") );
    connect( createPointAct, &QAction::triggered, this, &MainWindow::CreatePoint );

    createLineAct = new QAction( tr("&CreateLine"), this );
    createLineAct->setShortcuts( QKeySequence::New );
    createLineAct->setStatusTip( tr("Creating line") );
    connect( createLineAct, &QAction::triggered, this, &MainWindow::CreateLine );

    createCircleAct = new QAction( tr("&CreateCircle"), this );
    createCircleAct->setShortcuts( QKeySequence::New );
    createCircleAct->setStatusTip( tr("Creating circle") );
    connect( createCircleAct, &QAction::triggered, this, &MainWindow::CreateEllipse );

    createNurbsAct = new QAction( tr("&CreateNurbs"), this );
    createNurbsAct->setShortcuts( QKeySequence::New );
    createNurbsAct->setStatusTip( tr("Creating nurbs") );
    connect( createNurbsAct, &QAction::triggered, this, &MainWindow::CreateNurbs );

    deleteCurveAct = new QAction( tr("&DeleteCurve"), this );
    deleteCurveAct->setShortcuts( QKeySequence::New );
    deleteCurveAct->setStatusTip( tr("DeleteCurve") );
    connect( deleteCurveAct, &QAction::triggered, this, &MainWindow::DeleteCurve );

    findIntersectionAct = new QAction( tr("&Find Intersection"), this );
    findIntersectionAct->setShortcuts( QKeySequence::New );
    findIntersectionAct->setStatusTip( tr("Find Intersection") );
    connect( findIntersectionAct, &QAction::triggered, this, &MainWindow::FindIntersection );

    clearScreenAct = new QAction( tr("&Clear Screen"), this );
    clearScreenAct->setShortcuts( QKeySequence::New );
    clearScreenAct->setStatusTip( tr("Clear Screen") );
    connect( clearScreenAct, &QAction::triggered, this, &MainWindow::ClearScreen );


}

void MainWindow::SaveFile()
{
    FileIO save;
    save.Save();
}

void MainWindow::LoadFile()
{
    FileIO open;
    open.Open();
}

void MainWindow::CreatePoint()
{
    pointsfromscreen.sufficient = 1;
    creatorGeometry = new PointCreator();
}

void MainWindow::CreateLine()
{
    pointsfromscreen.sufficient = 2;
    creatorGeometry = new LineCreator();

}

void MainWindow::CreateEllipse()
{
    pointsfromscreen.sufficient = 3;
    creatorGeometry = new EllipseCreator();
}

void MainWindow::CreateNurbs()
{

}

void MainWindow::FindIntersection()
{

}

void MainWindow::ClearScreen()
{

}

void MainWindow::DeleteCurve()
{

}


void MainWindow::mousePressEvent( QMouseEvent *event )
{
    if ( pointsfromscreen.sufficient < 0 ) return;
    if ( pointsfromscreen.sufficient > 0 )
    {
        pointsfromscreen.points.push_back( event->pos() );
        PointCreator creator;
        creator.Create( chart, pointsfromscreen.points );
        pointsfromscreen.sufficient--;
        return;
    } else {
      creatorGeometry->Create( chart,  pointsfromscreen.points );
      pointsfromscreen.sufficient = -10;
      pointsfromscreen.points.resize(0);
    }
}
