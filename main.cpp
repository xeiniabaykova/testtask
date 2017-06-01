#include "Editor/MainWindow.h"
#include <QApplication>

#include <QtCharts/QChartView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a( argc, argv );

  QRect rec = QApplication::desktop()->screenGeometry();
  double height = rec.height();
  double width = rec.width();

  QChart *chart = new QChart();
  chart->setSizePolicy( QSizePolicy::Policy::Maximum,QSizePolicy::Policy::Maximum );
  chart->resize( width, height );
  Editor::MainWindow w( chart );
  w.CreateActions();
  w.CreateMenus();

  QChartView *chartView = new QChartView( chart );

  chartView->setRenderHint( QPainter::Antialiasing );

  w.setCentralWidget( chartView );
  w.resize( 600, 400 );
  w.show();

  return a.exec();
}
