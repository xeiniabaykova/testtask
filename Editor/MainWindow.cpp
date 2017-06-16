#include "MainWindow.h"
#include "FileIO.h"
#include <QLabel>
#include <QtWidgets/QMainWindow>
#include <Editor/ui_MainWindow.h>
#include <QInputDialog>
#include <QtWidgets/QVBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>


//------------------------------------------------------------------------------
// Конструктор MainWindow. Создается вспомогательный класс windowHandler  и форма MainWindow.
// ---
MainWindow::MainWindow( QChart *chart, QWidget *parent ) :
  windowHandler       ( chart              ),
  QMainWindow         ( parent             ),
  ui                  ( new Ui::MainWindow ),
  importExportMenu    ( 0                  ),
  createCurveMenu     ( 0                  ),
  optionsMenu         ( 0                  ),
  screenMenu          ( 0                  ),
  createPointAct      ( 0                  ),
  createLineAct       ( 0                  ),
  createEllipseAct    ( 0                  ),
  createPolylineAct   ( 0                  ),
  createNurbsAct      ( 0                  ),
  stopCreateCurveAct  ( 0                  ),
  findIntersectionAct ( 0                  ),
  deleteCurveAct      ( 0                  ),
  saveAct             ( 0                  ),
  loadAct             ( 0                  ),
  clearScreenAct      ( 0                  )
{
  ui->setupUi( this );
}

MainWindow::~MainWindow()
{
  delete ui;
}
#ifndef QT_NO_CONTEXTMENU


//------------------------------------------------------------------------------
// Создание контекстного меню с пунтктами: создать, удалить кривую. Поменять цвет кривой.
// ---
void MainWindow::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menu( this );

    stopCreateCurveAct = new QAction( tr("&stop create curve"), this );
    stopCreateCurveAct->setStatusTip( tr("stop sreating curve") );
    connect( stopCreateCurveAct, &QAction::triggered, this, &MainWindow::OnStopCreateCurve );

    reColorAct = new QAction( tr("&change color curve"), this );
    reColorAct->setStatusTip( tr("stop sreating curve") );
    connect( reColorAct, &QAction::triggered, this, &MainWindow::OnReColorCurve );



    menu.addAction( stopCreateCurveAct );
    menu.addAction( reColorAct );
    menu.addAction( deleteCurveAct );
    menu.exec( event->globalPos() );
}
#endif // QT_NO_CONTEXTMENU


//-----------------------------------------------------------------------------
//  Создаются меню : "File" с подменю "Open" "Save",
//  "Curves" с подменю "Create point", "Create Line", "Create Ellipse",
//    "Create Nurbs", "Create Circle"
//    "Options" с подменю "DeleteCurve" "Find Intersection"
//    "Screen"  с подменю "Clear Screen".
// ---
void MainWindow::CreateMenus()
{
  importExportMenu = menuBar()->addMenu( tr("&File") );
  importExportMenu->addAction( saveAct );
  importExportMenu->addAction( loadAct );

  createCurveMenu = menuBar()->addMenu( tr("&Curves") );
 // createCurveMenu->addAction( createPointAct );
  createCurveMenu->addAction( createLineAct );
  createCurveMenu->addAction( createEllipseAct );
  createCurveMenu->addAction( createPolylineAct );
  createCurveMenu->addAction( createNurbsAct );


  optionsMenu = menuBar()->addMenu( tr("&Options") );
  optionsMenu->addAction( deleteCurveAct );
  optionsMenu->addAction( findIntersectionAct) ;

  screenMenu = menuBar()->addMenu( tr("&Screen") );
  screenMenu->addAction( clearScreenAct );
}


//-----------------------------------------------------------------------------
//   Создаются действия для пунктов меню:
//   "save", "load", "create point", "create line", "create ellipse", "create Circle"
//   create nurbs", "delete curve", "find intersection", "Clear Screen".
// ---
void MainWindow::CreateActions()
{
  saveAct = new QAction( tr("&Save"), this );
  saveAct->setStatusTip( tr("Save into file") );
  connect( saveAct, &QAction::triggered, this, &MainWindow::OnSaveFile );

  loadAct = new QAction ( tr("&Load"), this);
  loadAct->setStatusTip( tr("Load from file") );
  connect( loadAct, &QAction::triggered, this, &MainWindow::OnLoadFile );


//  createPointAct = new QAction( tr("&Point"), this );
//  createPointAct->setStatusTip( tr("Creating point") );
//  createPointAct->setShortcut(tr("CTRL+Q"));
//  connect( createPointAct, &QAction::triggered, this, &MainWindow::OnCreatePoint );

  createLineAct = new QAction( tr("&Line"), this );
 // createLineAct->setShortcuts( QKeySequence::New );
  createLineAct->setStatusTip( tr("Creating line") );
  connect( createLineAct, &QAction::triggered, this, &MainWindow::OnCreateLine );

  createEllipseAct = new QAction( tr("&Ellipse"), this );
  createEllipseAct->setStatusTip( tr("Creating circle") );
  connect( createEllipseAct, &QAction::triggered, this, &MainWindow::OnCreateEllipse );

  createCircleAct  =  new QAction( tr("&Circle"), this );
  createCircleAct->setStatusTip( tr("Creating circle") );
  connect( createCircleAct, &QAction::triggered, this, &MainWindow::OnCreateCircle );

  createPolylineAct = new QAction( tr("&Poliline"), this );
  createPolylineAct->setStatusTip( tr("Creating nurbs") );
  connect( createPolylineAct, &QAction::triggered, this, &MainWindow::OnCreatePolyline );

  createNurbsAct = new QAction( tr("&Nurbs"), this );
  createNurbsAct->setStatusTip( tr("Creating nurbs") );
  connect( createNurbsAct, &QAction::triggered, this, &MainWindow::OnCreateNurbs );

  creatorCurves = new QActionGroup( this );
  creatorCurves->addAction( createLineAct ) ;
  creatorCurves->addAction( createEllipseAct );
  creatorCurves->addAction( createPolylineAct );
  creatorCurves->addAction( createNurbsAct );


  deleteCurveAct = new QAction( tr("&delete curve"), this );
  deleteCurveAct->setStatusTip( tr("delete curve") );
  connect( deleteCurveAct, &QAction::triggered, this, &MainWindow::OnDeleteCurve );

  findIntersectionAct = new QAction( tr("&Find Intersection"), this );
  findIntersectionAct->setStatusTip( tr("Find Intersection") );
  connect( findIntersectionAct, &QAction::triggered, this, &MainWindow::OnFindIntersection );

  clearScreenAct = new QAction( tr("&Clear Screen"), this );
  clearScreenAct->setStatusTip( tr("Clear Screen") );
  connect( clearScreenAct, &QAction::triggered, this, &MainWindow::OnClearScreen );
}


//-----------------------------------------------------------------------------
//  Обработчик события пункта меню "save"
// ---
void MainWindow::OnSaveFile()
{
  windowHandler.SaveFile();
}


//-----------------------------------------------------------------------------
//
//  Обработчик события пункта меню "load"
// ---
void MainWindow::OnLoadFile()
{
  windowHandler.LoadFile();
}


//-----------------------------------------------------------------------------
//  Обработчик события пункта меню "create point"
// ---
void MainWindow::OnCreatePoint()
{
  createPointAct->setCheckable( true );
  createPointAct->setChecked( true );
}


//-----------------------------------------------------------------------------
// Обработчик события пункта меню "create line".
// ---
void MainWindow::OnCreateLine()
{
  createLineAct->setCheckable( true );
  createLineAct->setChecked( true );
  windowHandler.CreateLine();
}


//-----------------------------------------------------------------------------
// Обработчик события пункта меню "create ellipse"
// ---
void MainWindow::OnCreateEllipse()
{
  createEllipseAct->setCheckable( true );
  createEllipseAct->setChecked( true );
  windowHandler.CreateEllipse();
}


//-----------------------------------------------------------------------------
//  Обработчик события пункта меню "create circle".
// ---

void MainWindow::OnCreateCircle()
{
  createCircleAct->setCheckable( true );
  createCircleAct->setChecked( true );
  windowHandler.CreateCircle();
}



//-----------------------------------------------------------------------------
// обработчик события пункта меню "create nurbs".
//---
void MainWindow::OnCreateNurbs()
{

  createNurbsAct->setCheckable( true );
  createNurbsAct->setChecked( true );
  windowHandler.CreateNurbs();
}


//-----------------------------------------------------------------------------
//   Обработчик события пункта меню "Stop create curve".
// ---
void MainWindow::OnStopCreateCurve()
{

  createPolylineAct->setCheckable( false );
  createPolylineAct->setChecked( false );
  windowHandler.StopCreateCurve();
}


//-----------------------------------------------------------------------------
//Обработчик события пункта меню "find intersection"
// ---
void MainWindow::OnFindIntersection()
{

}


//-----------------------------------------------------------------------------
//Обработчик события пункта меню "clear screen".
// ---
void MainWindow::OnClearScreen()
{
  windowHandler.ClearScreen();
}


//-----------------------------------------------------------------------------
//  Обработчик события пункта меню "delete curve".
// ---
void MainWindow::OnDeleteCurve()
{
  createPolylineAct->setCheckable( false );
  createPolylineAct->setChecked( false );
  windowHandler.DeleteCurve();
}

void MainWindow::OnCreatePolyline()
{
  createPolylineAct->setCheckable( true );
  createPolylineAct->setChecked( true );
  windowHandler.CreatePolyline();
}


//-----------------------------------------------------------------------------
// Обработчик события клика мышки.
// ---
void MainWindow::mousePressEvent( QMouseEvent *event )
{
  windowHandler.MouseEvent( event );
}


//-----------------------------------------------------------------------------
// Обработчик события изменения цвета кривой.
// ---
void MainWindow::OnReColorCurve()
{
  QDialog * d = new QDialog();
  QVBoxLayout * vbox = new QVBoxLayout();

  QLabel* RLabel = new QLabel( "&R:", this );
  QLabel* GLabel = new QLabel( "&G:", this );
  QLabel* BLabel = new QLabel( "&B:", this );
  QLineEdit * r = new QLineEdit();
  QLineEdit * g = new QLineEdit();
  QLineEdit * b = new QLineEdit();
  QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok| QDialogButtonBox::Cancel );

  QObject::connect( buttonBox, SIGNAL(accepted()), d, SLOT(accept()) );
  QObject::connect( buttonBox, SIGNAL(rejected()), d, SLOT(reject()) );
  RLabel->setBuddy( r );
  GLabel->setBuddy( g );
  BLabel->setBuddy( b );

  vbox->addWidget( RLabel );
  vbox->addWidget( r );
  vbox->addWidget( GLabel );
  vbox->addWidget( g );
  vbox->addWidget( BLabel );
  vbox->addWidget( b );

  vbox->addWidget( buttonBox );

  d->setLayout( vbox );

  int result = d->exec();

  QColor newColor;
  if ( result == QDialog::Accepted )
  {
   newColor.setRed( r->text().toInt() );
   newColor.setGreen( g->text().toInt() );
   newColor.setBlue( b->text().toInt() );
  }
  windowHandler.ChangeColor( newColor );
}


//-----------------------------------------------------------------------------
// Обработчик изменения размера окна.
// ---
void MainWindow::resizeEvent( QResizeEvent *event )
{
  windowHandler.ResizeEvent ( event );
}


//-----------------------------------------------------------------------------
// Обработчик контекстного меню.
// ---
void MainWindow::ContextMenuRequested( const QPoint& point )
{
  contextMenu->popup( mapToGlobal(point) );
}
