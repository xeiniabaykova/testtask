#include "MainWindow.h"
#include "FileIO.h"
#include <QLabel>
#include <QtWidgets/QMainWindow>
#include <ui_MainWindow.h>
#include <QInputDialog>
#include <QtWidgets/QVBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>

//------------------------------------------------------------------------------
// \ru Конструктор MainWindow. Создается вспомогательный класс windowHandler  и форма MainWindow
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
  createCircleAct     ( 0                  ),
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
 // createCurveMenu->addAction( createPointAct );
  createCurveMenu->addAction( createLineAct.get() );
  createCurveMenu->addAction( createEllipseAct );
  createCurveMenu->addAction( createPolylineAct );
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
  saveAct->setStatusTip( tr("Save into file") );
  connect( saveAct, &QAction::triggered, this, &MainWindow::OnSaveFile );

  loadAct = new QAction ( tr("&Load"), this);
  loadAct->setStatusTip( tr("Load from file") );
  connect( loadAct, &QAction::triggered, this, &MainWindow::OnLoadFile );


  createPointAct = new QAction ( tr("&Point"), this );
  createPointAct->setStatusTip( tr("Creating point") );
  createPointAct->setShortcut(tr("CTRL+Q"));
  connect( createPointAct, &QAction::triggered, this, &MainWindow::OnCreatePoint );

  createLineAct = std::make_shared<QAction>( tr("&Line"), this );
 // createLineAct->setShortcuts( QKeySequence::New );
  createLineAct->setStatusTip( tr("Creating line") );
  connect( createLineAct.get(), &QAction::triggered, this, &MainWindow::OnCreateLine );

  createEllipseAct = new QAction( tr("&Ellipse"), this );
  createEllipseAct->setStatusTip( tr("Creating circle") );
  connect( createEllipseAct, &QAction::triggered, this, &MainWindow::OnCreateEllipse );

  createPolylineAct = new QAction( tr("&Polyline"), this );
 // createPolylineAct->setShortcuts( QKeySequence::New );
  createPolylineAct->setStatusTip( tr("Creating circle") );
  connect( createPolylineAct, &QAction::triggered, this, &MainWindow::OnCreatePolyline );

  createNurbsAct = new QAction( tr("&Nurbs"), this );
  createNurbsAct->setStatusTip( tr("Creating nurbs") );
  connect( createNurbsAct, &QAction::triggered, this, &MainWindow::OnCreateNurbs );

  createCircleAct = new QAction( tr("&Circle"), this );
  createCircleAct->setDisabled( true );
 // createCircleAct->setShortcuts( QKeySequence::New );
  createCircleAct->setStatusTip( tr("Creating Circle") );
  connect( createCircleAct, &QAction::triggered, this, &MainWindow::OnCreateCircle );


  creatorCurves = new QActionGroup( this );
//  creatorCurves->addAction( createPointAct );
  creatorCurves->addAction( createLineAct.get() ) ;
  creatorCurves->addAction( createEllipseAct );
//  creatorCurves->addAction( createCircleAct );
  creatorCurves->addAction( createPolylineAct );
  creatorCurves->addAction( createNurbsAct );


  deleteCurveAct = new QAction( tr("&delete curve"), this );
 // deleteCurveAct->setShortcuts( QKeySequence::New );
  deleteCurveAct->setStatusTip( tr("delete curve") );
  connect( deleteCurveAct, &QAction::triggered, this, &MainWindow::OnDeleteCurve );

  findIntersectionAct = new QAction( tr("&Find Intersection"), this );
 // findIntersectionAct->setShortcuts( QKeySequence::New );
  findIntersectionAct->setStatusTip( tr("Find Intersection") );
  connect( findIntersectionAct, &QAction::triggered, this, &MainWindow::OnFindIntersection );

  clearScreenAct = new QAction( tr("&Clear Screen"), this );
 // clearScreenAct->setShortcuts( QKeySequence::New );
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
  //windowHandler.CreatePoint();
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

  createNurbsAct->setCheckable( true );
  createNurbsAct->setChecked( true );
  windowHandler.CreateNurbs();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "Stop create curve"

*/
//-----------------------------------------------------------------------------
void MainWindow::OnStopCreateCurve()
{

  createPolylineAct->setCheckable( false );
  createPolylineAct->setChecked( false );
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
  windowHandler.ClearScreen();
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события пункта меню "delete curve"

*/
//-----------------------------------------------------------------------------
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
/**
   \ru обработчик события клика мышки

*/
//-----------------------------------------------------------------------------
void MainWindow::mousePressEvent( QMouseEvent *event )
{
  windowHandler.MouseEvent( event );
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик события изменения цвета кривой

*/
//-----------------------------------------------------------------------------
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
/**
   \ru обработчик изменения размера окна

*/
//-----------------------------------------------------------------------------
void MainWindow::resizeEvent( QResizeEvent *event )
{
  windowHandler.ResizeEvent ( event );
}


//-----------------------------------------------------------------------------
/**
   \ru обработчик контекстного меню
*/
//-----------------------------------------------------------------------------
void MainWindow::contextMenuRequested( const QPoint& point )
{
  contextMenu->popup( mapToGlobal(point) );
}
