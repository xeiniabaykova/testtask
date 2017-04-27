#include "MainWindow.h"
#include "FileIO.h"
#include <QLabel>
#include <QtWidgets/QMainWindow>
#include <ui_MainWindow.h>

//------------------------------------------------------------------------------
// \ru Конструктор MainWindow. Создается вспомогательный класс windowHandler  и форма MainWindow
// ---
MainWindow::MainWindow( QChart *chart, QWidget *parent ) :
  windowHandler( chart ),
  QMainWindow( parent ),
  ui( new Ui::MainWindow ),
  importExportMenu(0),
  createCurveMenu(0),
  optionsMenu(0),
  screenMenu(0),
  createPointAct(0),
  createLineAct(0),
  createEllipseAct(0),
  createCircleAct(0),
  createNurbsAct(0),
  stopCreateCurveAct(0),
  findIntersectionAct(0),
  deleteCurveAct(0),
  saveAct(0),
  loadAct(0),
  clearScreenAct(0)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    stopCreateCurveAct = new QAction( tr("&stop create curve"), this );
    stopCreateCurveAct->setShortcuts( QKeySequence::New );
    stopCreateCurveAct->setStatusTip( tr("stop sreating curve") );
    connect( stopCreateCurveAct, &QAction::triggered, this, &MainWindow::OnStopCreateCurve );
    menu.addAction(stopCreateCurveAct);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
//-----------------------------------------------------------------------------
/**
    \ru создаются меню : "File" с подменю "Open" "Save",
    \ru "Curves" с подменю "Create point", "Create Line", "Create Ellipse",
    \ru "Create Nurbs", "Create Circle"
    \ru "Options" с подменю "DeleteCurve" "Find Intersection"
    \ru "Screen"  с подменю "Clear Screen"

*/
//-----------------------------------------------------------------------------
void MainWindow::CreateMenus()
{
  importExportMenu = menuBar()->addMenu( tr("&File") );
  importExportMenu->addAction( saveAct );
  importExportMenu->addAction( loadAct );

  createCurveMenu = menuBar()->addMenu( tr("&Curves") );
  createCurveMenu->addAction( createPointAct );
  createCurveMenu->addAction( createLineAct );
  createCurveMenu->addAction( createEllipseAct );
  createCurveMenu->addAction( createNurbsAct );
  createCurveMenu->addAction( createCircleAct );


  optionsMenu = menuBar()->addMenu( tr("&Options") );
  optionsMenu->addAction( deleteCurveAct );
  optionsMenu->addAction( findIntersectionAct) ;

  screenMenu = menuBar()->addMenu( tr("&Screen") );
  screenMenu->addAction( clearScreenAct );
}


//-----------------------------------------------------------------------------
/**
   \ru создаются действия для пунктов меню:
   \ru "save", "load", "create point", "create line", "create ellipse", "create Circle"
   \ru "create nurbs", "delete curve", "find intersection", "Clear Screen"

*/
//-----------------------------------------------------------------------------
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


  createPointAct = new QAction ( tr("&Point"), this );
  createPointAct->setShortcuts( QKeySequence::New );
  createPointAct->setStatusTip( tr("Creating point") );
  createPointAct->setShortcut(tr("CTRL+Q"));
  connect( createPointAct, &QAction::triggered, this, &MainWindow::OnCreatePoint );

  createLineAct = new QAction( tr("&Line"), this );
  createLineAct->setShortcuts( QKeySequence::New );
  createLineAct->setStatusTip( tr("Creating line") );
  connect( createLineAct, &QAction::triggered, this, &MainWindow::OnCreateLine );

  createEllipseAct = new QAction( tr("&Ellipse"), this );
  createEllipseAct->setShortcuts( QKeySequence::New );
  createEllipseAct->setStatusTip( tr("Creating circle") );
  connect( createEllipseAct, &QAction::triggered, this, &MainWindow::OnCreateEllipse );

  createNurbsAct = new QAction( tr("&Nurbs"), this );
  createNurbsAct->setShortcuts( QKeySequence::New );
  createNurbsAct->setStatusTip( tr("Creating nurbs") );
  connect( createNurbsAct, &QAction::triggered, this, &MainWindow::OnCreateNurbs );

  createCircleAct = new QAction( tr("&Circle"), this );
  createCircleAct->setShortcuts( QKeySequence::New );
  createCircleAct->setStatusTip( tr("Creating Circle") );
  connect( createCircleAct, &QAction::triggered, this, &MainWindow::OnCreateCircle );


  creatorCurves = new QActionGroup(this);
  creatorCurves->addAction( createPointAct );
  creatorCurves->addAction( createLineAct ) ;
  creatorCurves->addAction( createEllipseAct );
  creatorCurves->addAction( createCircleAct );
  creatorCurves->addAction( createNurbsAct );


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


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "save"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnSaveFile()
{
  windowHandler.SaveFile();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "load"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnLoadFile()
{
  windowHandler.LoadFile();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "create point"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnCreatePoint()
{
  createPointAct->setCheckable( true );
  createPointAct->setChecked( true );
  windowHandler.CreatePoint();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "create line"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnCreateLine()
{
  createLineAct->setCheckable( true );
   createLineAct->setChecked( true );
   windowHandler.CreateLine();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "create ellipse"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnCreateEllipse()
{
  createEllipseAct->setCheckable( true );
  createEllipseAct->setChecked( true );
  windowHandler.CreateEllipse();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "create circle"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnCreateCircle()
{
  createCircleAct->setCheckable( true );
  createCircleAct->setChecked( true );
  windowHandler.CreateCircle();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "create nurbs"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnCreateNurbs()
{

}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "Stop create curve"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnStopCreateCurve()
{

  createPointAct->setCheckable( true );
  createPointAct->setChecked( true );
  createPointAct->setCheckable( false );
  createPointAct->setChecked( false );
  windowHandler.StopCreateCurve();
}

//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "find intersection"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnFindIntersection()
{

}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "clear screen"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnClearScreen()
{

}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "delete curve"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnDeleteCurve()
{
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события клика мышки

*/
//-----------------------------------------------------------------------------
void MainWindow::mousePressEvent( QMouseEvent *event )
{
  windowHandler.MouseEvent( event );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

  windowHandler.ResizeEvent ( event );
}


void MainWindow::contextMenuRequested(const QPoint& point)
{
    contextMenu->popup(mapToGlobal(point));
}
