#include "chartview.h"
#include <QtGui/QMouseEvent>

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent)
{
    setRubberBand(QChartView::RectangleRubberBand);
}


void ChartView::mousePressEvent( QMouseEvent *event )
{

}



