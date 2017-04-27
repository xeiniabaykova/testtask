#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "FileIO.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDesktopWidget>
#include <QtCharts/QScatterSeries>
#include <algorithm>



//-----------------------------------------------------------------------------
/**
  \ru Конструктор MainWindowHandler
*/
//-----------------------------------------------------------------------------
MainWindowHandler::MainWindowHandler (QChart * chart):
  chart( chart ),
  printChart( chart ),
  points(0),
  selector( geomPolylines ),
  GeomCreator(0, nullptr ) {}


//-----------------------------------------------------------------------------
/**
  \ru добавление точки с экрана в массив точек
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::AddPointFromScreen( Point point )
{
  points.push_back( point );
}


//-----------------------------------------------------------------------------
/**
 \ru  возвращает количество точек, необходимое для отрисовки текущего геометрического примитива
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::AddSufficientNum ( int num )
{
  GeomCreator.sufficient = num;
}


//-----------------------------------------------------------------------------
/**
   \ru проверка, достаточно точек на экране выбрано для построения кривой

*/
//-----------------------------------------------------------------------------
bool MainWindowHandler::IsSufficientNum()
{
  return ( points.size() == GeomCreator.sufficient - 1 );
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - 1
  \ru ruсоздается объект для создания геометрического представления точки

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreatePoint()
{
  state = StateCreateCurve;
  GeomCreator.sufficient = 1;
  GeomCreator.creator = new PointCreator();
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - 2
  \ru создается объект для создания геометрического представления линии

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateLine()
{
  state = StateCreateCurve;
  GeomCreator.sufficient = 2;
  GeomCreator.creator = new LineCreator();
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - 2
  \ru создается объект для создания геометрического представления эллипса

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateEllipse()
{
  state = StateCreateCurve;
  GeomCreator.sufficient = 3;
  GeomCreator.creator = new EllipseCreator();
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - 2
  \ru создается объект для создания геометрического представления nurbs

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateNurbs()
{
// state = CreateCurve;
}


//-----------------------------------------------------------------------------
/**
  \ru создается объект для открытия файла
  \ru запускается окно открытия файла
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::LoadFile()
{

  FileIO open;
  open.Open();
}


//-----------------------------------------------------------------------------
/**
  \ru создается объект для сохранения файла
  \ru запускается окно сохранения файла
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::SaveFile()
{

  FileIO save;
  save.Save();
}


//-----------------------------------------------------------------------------
/**
  \ru вызывается метод создания кривой по точкам
  \ru обнуляется массив точек полученных с экрана
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateCurve( std::vector<QXYSeries*> currentSeriesPoint )
{
  DisplayChartCurve curve;
  screenCurves.push_back(curve);
  std::vector<Point> currentPoints;
  GeomCreator.creator->Create( points )->GetAsPolyLine( currentPoints, 0.01 );
  geomPolylines.push_back( currentPoints );
  int num = std::max((int)(screenCurves.size() - 1), 0);
  if (currentPoints.size() == 1)
    screenCurves[num].referencePoints.push_back(printChart.AddFigure( currentPoints ));
  else {
    screenCurves[num].chartPoints.push_back(printChart.AddFigure( currentPoints ));
    screenCurves[num].referencePoints = currentSeriesPoint;
 }
  screenPolyIndexes.insert( std::pair<int, int> (geomPolylines.size() - 1, num) );
  points.resize(0);

}

void MainWindowHandler::StopCreateCurve()
{
  state = StateExpectAction;
}

//-----------------------------------------------------------------------------
/**
  \ru обработка события клика мышкой
  \ru если количество точек достаточно для создания крвой, то она создается
  \ru если недостаточно, точка добавляется в массив
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::MouseEvent( QMouseEvent *event )
{
  std::vector<QXYSeries*> currentSeriesPoint;
  if (state == StateCreateCurve) {
     if ( !IsSufficientNum() )
     {
       QPointF currentPoint = chart->mapToValue(event->pos());
       AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
       PrintCharacteristicPoint ( Point(currentPoint.x(), currentPoint.y()), currentSeriesPoint );
     } else {
       QPointF currentPoint = chart->mapToValue(event->pos());
       AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
       PrintCharacteristicPoint ( Point(currentPoint.x(), currentPoint.y()), currentSeriesPoint );
       CreateCurve(currentSeriesPoint);
     }
  } else if (state == StateExpectAction) {
    QPointF currentPoint = chart->mapToValue( QPointF(event->pos().x(), event->pos().y()) );
    int selectSeries = selector.GetCurve( Point (currentPoint.x(), currentPoint.y()) );
    if ( selectSeries != -1 ) {
      DisplayChartCurve chartCurve =  screenCurves[screenPolyIndexes.at(selectSeries)];
      isSelected.push_back(selectSeries);
      for ( int i = 0; i< chartCurve.chartPoints.size(); i++)
          chartCurve.chartPoints[i]->setColor( QColor(153, 255, 255) );
      for ( int i = 0; i< chartCurve.referencePoints.size(); i++)
          chartCurve.referencePoints[i]->setColor( QColor(153, 255, 255) );
  }
    state = StateExpectAction;
  }
}


void  MainWindowHandler::MouseDoubleClickEvent( QMouseEvent *event )
{
  for ( int j = 0; j< isSelected.size(); j++ )
  {
    DisplayChartCurve chartCurve =  screenCurves[screenPolyIndexes.at(j)];
    for ( int i = 0; i< chartCurve.chartPoints.size(); i++)
        chartCurve.chartPoints[i]->setColor( QColor(51, 0, 51) );
    for ( int i = 0; i< chartCurve.referencePoints.size(); i++)
        chartCurve.referencePoints[i]->setColor( QColor(51, 0, 51) );

  }
  isSelected.resize(0);

}

void MainWindowHandler::PrintCharacteristicPoint( Point point, std::vector<QXYSeries*>& currentSeriesPoint )
{
  PointCreator *creator = new PointCreator();
  std::vector<Point> currentPoint;
  currentPoint.push_back( point );
  std::vector<Point> currentPoints;
  creator->Create( currentPoint )->GetAsPolyLine( currentPoints, 0.01 );
  currentSeriesPoint.push_back( printChart.AddFigure( currentPoints ) );
}


void MainWindowHandler::ResizeEvent( QResizeEvent *event )
{
  QRect rec = QApplication::desktop()->screenGeometry();
   double height = rec.height();
   double width = rec.width();
   chart->resize( width, height );

}
