#include "Editor/MainWindow.h"
#include <QApplication>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMainWindow>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a( argc, argv );

  QChart *chart = new QChart();


  MainWindow w( chart );
  w.CreateActions();
  w.CreateMenus();

  QLineSeries *series0 = new QLineSeries();
  chart->legend()->setVisible(false);
  chart->addSeries(series0);

  QChartView *chartView = new QChartView(chart);

  chartView->setRenderHint(QPainter::Antialiasing);

  w.setCentralWidget(chartView);
  w.resize(400, 300);
  w.show();

      return a.exec();
}
