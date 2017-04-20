#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FileIO.h"
#include <QLabel>
#include <QtWidgets/QMainWindow>




MainWindow::MainWindow( QChart *chart, QWidget *parent ) :
  windowHandler( chart ),
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
  connect( saveAct, &QAction::triggered, this, &MainWindow::OnSaveFile );

  loadAct = new QAction ( tr("&Load"), this);
  loadAct->setShortcuts( QKeySequence::New );
  loadAct->setStatusTip( tr("Load from file") );
  connect( loadAct, &QAction::triggered, this, &MainWindow::OnLoadFile );

  createPointAct = new QAction ( tr("&CreatePoint"), this );
  createPointAct->setShortcuts( QKeySequence::New );
  createPointAct->setStatusTip( tr("Creating point") );
  connect( createPointAct, &QAction::triggered, this, &MainWindow::OnCreatePoint );

  createLineAct = new QAction( tr("&CreateLine"), this );
  createLineAct->setShortcuts( QKeySequence::New );
  createLineAct->setStatusTip( tr("Creating line") );
  connect( createLineAct, &QAction::triggered, this, &MainWindow::OnCreateLine );

  createCircleAct = new QAction( tr("&CreateCircle"), this );
  createCircleAct->setShortcuts( QKeySequence::New );
  createCircleAct->setStatusTip( tr("Creating circle") );
  connect( createCircleAct, &QAction::triggered, this, &MainWindow::OnCreateEllipse );

  createNurbsAct = new QAction( tr("&CreateNurbs"), this );
  createNurbsAct->setShortcuts( QKeySequence::New );
  createNurbsAct->setStatusTip( tr("Creating nurbs") );
  connect( createNurbsAct, &QAction::triggered, this, &MainWindow::OnCreateNurbs );

  deleteCurveAct = new QAction( tr("&DeleteCurve"), this );
  deleteCurveAct->setShortcuts( QKeySequence::New );
  deleteCurveAct->setStatusTip( tr("DeleteCurve") );
  connect( deleteCurveAct, &QAction::triggered, this, &MainWindow::OnDeleteCurve );

  findIntersectionAct = new QAction( tr("&Find Intersection"), this );
  findIntersectionAct->setShortcuts( QKeySequence::New );
  findIntersectionAct->setStatusTip( tr("Find Intersection") );
  connect( findIntersectionAct, &QAction::triggered, this, &MainWindow::OnFindIntersection );

  clearScreenAct = new QAction( tr("&Clear Screen"), this );
  clearScreenAct->setShortcuts( QKeySequence::New );
  clearScreenAct->setStatusTip( tr("Clear Screen") );
  connect( clearScreenAct, &QAction::triggered, this, &MainWindow::OnClearScreen );


}

void MainWindow::OnSaveFile()
{
  windowHandler.SaveFile();
}

void MainWindow::OnLoadFile()
{
  windowHandler.LoadFile();
}

void MainWindow::OnCreatePoint()
{
  windowHandler.CreatePoint();
}

void MainWindow::OnCreateLine()
{
  windowHandler.CreateLine();
}

void MainWindow::OnCreateEllipse()
{
  windowHandler.CreateEllipse();
}

void MainWindow::OnCreateNurbs()
{

}

void MainWindow::OnFindIntersection()
{

}

void MainWindow::OnClearScreen()
{

}

void MainWindow::OnDeleteCurve()
{
}


void MainWindow::mousePressEvent( QMouseEvent *event )
{
  windowHandler.MouseEvent( event );
}

