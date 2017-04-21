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

      //*series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
         //      << QPointF(16, 7) << QPointF(18, 5);


      chart->addSeries(series0);
      chart->createDefaultAxes();
     // chart->axisX()->setRange(0, 20);
    //  chart->axisY()->setRange(0, 10);

      QChartView *chartView = new QChartView(chart);
      chartView->setRenderHint(QPainter::Antialiasing);


      w.setCentralWidget(chartView);
      w.resize(400, 300);
      w.show();

      return a.exec();
}
