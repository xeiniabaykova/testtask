#ifndef PRINTFUGURE_H
#define PRINTFUGURE_H
#include "Math/Point.h"
#include <QtCharts/QChart>
#include <QtCharts/QXYSeries>
#include <QtCharts/QValueAxis>
#include <Editor/DisplayedCurve.h>
#include <vector>


QT_CHARTS_USE_NAMESPACE

//------------------------------------------------------------------------------
/** \brief \ru Класс для отображения графика в chart
 * на вход получает набор точек и отбображает их\~
**/
// ---

class PrintFigure {

private:
  QChart * chart;  ///< для отображения графика
  QValueAxis * axisX;
  QValueAxis * axisY;
public:
  //-------------------------------------------------------------------------------
  // \ru chart - на вход передается средство для отбражения графика
  // ---
  PrintFigure( QChart * chart );
  //-------------------------------------------------------------------------------
  // \ru на вход передается массив точек для отображения
  // ---
  void AddFigure( DisplayedCurve * curve, QColor color = QColor (51, 0 , 51) );  ///< добавление новой кривой
};
#endif // PRINTFUGURE_H
