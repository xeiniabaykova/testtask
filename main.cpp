#include "Editor/MainWindow.h"
#include <QApplication>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

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
   chart->createDefaultAxes();
  MainWindow w( chart );
  w.CreateActions();
  w.CreateMenus();

  QLineSeries *series0 = new QLineSeries();
  chart->legend()->setVisible(false);
  chart->addSeries(series0);

  QChartView *chartView = new QChartView(chart);

  chartView->setRenderHint(QPainter::Antialiasing);

  w.setCentralWidget(chartView);
  w.resize( 600, 800 );
  w.show();

  return a.exec();
}
