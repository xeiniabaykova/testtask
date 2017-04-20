#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

#include "collectpoints.h"
#include "GeometricPrimitiveCreator.h"
#include <QtCharts/QChartView>
#include <QPoint>
#include <vector>

QT_CHARTS_USE_NAMESPACE
///////////////////////////////////////////////////////////////////////////////
//   Класс обработчик событий формы mainwindow
///
/**
  Класс реализует логику обработчиков с формы
*/
///////////////////////////////////////////////////////////////////////////////


class MainWindowHandler {

private:
    CollectPoints          pointsFromScreen;
    QChart             *   chart;
    int                    sufficient;
    GeometricPrimitiveCreator * creator;
public:
    MainWindowHandler (QChart * chart);

    void AddPointFromScreen( QPoint point );
    void AddSufficientNum (int num);
    bool IsSufficientNum ();

    void              CreatePoint    ();
    void              CreateLine     ();
    void              CreateEllipse  ();
    void              CreateNurbs    ();

    void              LoadFile        ();
    void              SaveFile        ();
    void              CreateCurve     ();

};

#endif // MAINWINDOWHANDLER_H
