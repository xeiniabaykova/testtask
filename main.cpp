#include "MainWindow.h"
#include <QApplication>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->createDefaultAxes();

    //chartView->setRenderHint(QPainter::Antialiasing);
    MainWindow w(chart);
    w.CreateActions();
    w.CreateMenus();
    QChartView *chartView = new QChartView( chart );
    w.setCentralWidget( chartView );
    w.show();

    return a.exec();
}
