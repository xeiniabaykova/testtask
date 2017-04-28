#ifndef PRINTFUGURE_H
#define PRINTFUGURE_H
#include "Math/Point.h"
#include <QtCharts/QChart>
#include <QtCharts/QXYSeries>
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
public:
  //-------------------------------------------------------------------------------
  // \ru chart - на вход передается средство для отбражения графика
  // ---
  PrintFigure( QChart * chart );
  //-------------------------------------------------------------------------------
  // \ru на вход передается массив точек для отображения
  // ---
  QXYSeries* AddFigure( const std::vector<Point>& points );  ///< добавление новой кривой
};
#endif // PRINTFUGURE_H
