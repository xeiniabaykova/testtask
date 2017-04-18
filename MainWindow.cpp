#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    createCurveMenu = menuBar()->addMenu( tr("&Create Curve") );
    createCurveMenu->addAction ( createPointAct );
    createCurveMenu->addAction ( createLineAct );
    createCurveMenu->addAction ( createCircleAct );
    createCurveMenu->addAction ( createNurbsAct );


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

    createLineAct = new QAction ( tr("&CreateLine"), this );
    createLineAct->setShortcuts( QKeySequence::New );
    createLineAct->setStatusTip( tr("Creating line") );
    connect( createLineAct, &QAction::triggered, this, &MainWindow::CreateLine );

    createCircleAct = new QAction ( tr("&CreateCircle"), this );
    createCircleAct->setShortcuts( QKeySequence::New );
    createCircleAct->setStatusTip( tr("Creating circle") );
    connect( createCircleAct, &QAction::triggered, this, &MainWindow::CreateCircle );

    createNurbsAct = new QAction ( tr("&CreateNurbs"), this );
    createNurbsAct->setShortcuts( QKeySequence::New );
    createNurbsAct->setStatusTip( tr("Creating nurbs") );
    connect( createNurbsAct, &QAction::triggered, this, &MainWindow::CreateNurbs );


}

void MainWindow::SaveFile()
{

}

void MainWindow::LoadFile()
{
}

void MainWindow::CreatePoint()
{

}

void MainWindow::CreateLine()
{

}

void MainWindow::CreateCircle()
{

}

void MainWindow::CreateNurbs()
{

}
