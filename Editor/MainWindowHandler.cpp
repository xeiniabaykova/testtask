#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "FileIO.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDesktopWidget>
#include <QInputDialog>
#include <fstream>
#include <functional>

//#include <QtCharts/QScatterSeries>
//#include <algorithm>



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
  GeomCreator(0, nullptr ),
  state (StateExpectAction),
  currentColor( 153, 255, 255 ),
  selectingColor( 51, 0, 51 ){}


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
  \ru задается необходимое количество точек - 3
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
  \ru создается объект для создания геометрического представления окружности

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateCircle()
{
  state = StateCreateCurve;
  GeomCreator.sufficient = 2;
//  GeomCreator.creator = new CircleCreator();
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
void MainWindowHandler::CreateCurve( std::vector<Point> referenceSeriesPoint )
{

  std::vector<Point> currentPoints;
  if (state == StateCreatePolyline) {
    geomPolylines.push_back( points );
    printChart.AddFigure( points, referenceSeriesPoint );
  } else {
  GeomCreator.creator->Create( points )->GetAsPolyLine( currentPoints, 0.001 );
  geomPolylines.push_back( currentPoints );
  geomReferencedPoints.push_back( referenceSeriesPoint );
  printChart.AddFigure( currentPoints, referenceSeriesPoint );
  }
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
   if( event->buttons() == Qt::RightButton )
     return;

    QRect rec = QApplication::desktop()->screenGeometry();
    double height = rec.height();
    double width = rec.width();
    chart->resize( width, height );

  if ( state == StateCreateCurve || state == StateCreatePolyline ) {
     if ( !IsSufficientNum() )
     {
       QPointF currentPoint = chart->mapToValue(event->pos());
       AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
       currentSeriesPoint.push_back( Point(currentPoint.x(), currentPoint.y()) );
     } else {
       QPointF currentPoint = chart->mapToValue(event->pos());
       AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
       currentSeriesPoint.push_back( Point(currentPoint.x(), currentPoint.y()) );
       CreateCurve( currentSeriesPoint );
       currentSeriesPoint.resize(0);
     }
  } else if (state == StateExpectAction) {

  StateExpect ( event );
  }
}

void MainWindowHandler::StateExpect( QMouseEvent *event )
{
  QPointF currentPoint = chart->mapToValue( QPointF(event->pos().x(), event->pos().y() - 30) );
  int selectSeries = selector.GetCurve( Point (currentPoint.x(), currentPoint.y()) );
  if ( selectSeries != -1 ) {
    isSelected.push_back(selectSeries);
    std::vector<Point> currentPoints;
     chart->removeAllSeries();
     for ( int i = 0; i < geomPolylines.size(); i++ )
     {
       if (std::find (isSelected.begin(), isSelected.end(), i ) == isSelected.end() ){
         printChart.AddFigure( geomPolylines[i], geomReferencedPoints[i], selectingColor );
       } else
         printChart.AddFigure( geomPolylines[i], geomReferencedPoints[i], currentColor );
     }
  }

if ( selectSeries == -1 ){
  chart->removeAllSeries();
  for ( int j = 0; j < geomPolylines.size(); j++ )
  {

      printChart.AddFigure( geomPolylines[j], geomReferencedPoints[j], selectingColor );
  }
  isSelected.resize(0);
}
  state = StateExpectAction;

}
void  MainWindowHandler::ChangeColor( QColor color ){

  for (int i = 0; i < isSelected.size(); i++) {
   printChart.AddFigure( geomPolylines[i], geomReferencedPoints[i], color );
  }

}
void MainWindowHandler::DeleteCurve()
{
   std::sort( isSelected.begin(), isSelected.end(), std::greater<int>() );
   for ( int i = 0; i < isSelected.size(); i++ )
   {
   geomPolylines.erase( geomPolylines.begin() + isSelected[i] );
   geomReferencedPoints.erase( geomReferencedPoints.begin() + isSelected[i] );
   }
   chart->removeAllSeries();
   for ( int i = 0; i< geomPolylines.size(); i++)
      printChart.AddFigure( geomPolylines[i], geomReferencedPoints[i], selectingColor );
   isSelected.resize(0);
}

void MainWindowHandler::ResizeEvent( QResizeEvent *event )
{
  QRect rec = QApplication::desktop()->screenGeometry();
   double height = rec.height();
   double width = rec.width();
    chart->resize( width, height );

}

 void MainWindowHandler::CreatePolyline()
 {
   bool ok;
       int i = QInputDialog::getInt(0, ("Polyline Points"),
                                    ("amount of points"), 25, 0, 100, 1, &ok);
       if (ok)
           GeomCreator.sufficient = i;
       state = StateCreatePolyline;
 }
