#ifndef MAINWINDOWHANDLER_H
#define MAINWINDOWHANDLER_H

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
public:
  struct Creator {
    int                         sufficient;
    GeometricPrimitiveCreator * creator;
  };
private:
  std::vector<QPoint>  points;
  QChart             * chart;
  Creator              GeomCreator;
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
  void              MouseEvent( QMouseEvent *event );

};

#endif // MAINWINDOWHANDLER_H
