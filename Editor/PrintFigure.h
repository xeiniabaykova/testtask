////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru рисователь отдельной кривой\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef PRINTFUGURE_H
#define PRINTFUGURE_H

#include "Math/Point.h"
#include <QtCharts/QChart>
#include <QtCharts/QXYSeries>
#include <QtCharts/QScatterSeries>
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
public:
   PrintFigure( QChart * chart );
  ~PrintFigure() = default;

private:
  PrintFigure( const PrintFigure &obj ) = delete;
  PrintFigure& operator=( PrintFigure &obj ) = delete;

private:
  QChart         * chart;            ///< для отображения графика
  QValueAxis     * axisX;            ///< ость х
  QValueAxis     * axisY;            ///< ость y
  QScatterSeries * seriesReferenced; ///< для отображения точек, выбранных пользователем

public:
  void AddFigure( std::shared_ptr<DisplayedCurve> curve, QColor color = QColor (51, 0 , 51) );  ///< добавить новую кривую на график
  void AddReferencedPoint( Point point, QColor color = QColor (51, 0 , 51) );                   ///< добавить точку, выбранную пользователем на экране

};
#endif // PRINTFUGURE_H
