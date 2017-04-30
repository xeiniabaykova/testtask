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
#include <QtCharts/QLineSeries>
#include "GeomPolylineCreator.h"

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
  points( 0 ),
  selector( geomPolylines ),
  geomCreator( 0, nullptr ),
  state ( StateExpectAction ),
  currentColor( 153, 255, 255 ),
  selectingColor( 51, 0, 51 )
{
}


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
  geomCreator.numExpectedPoits = num;
}


//-----------------------------------------------------------------------------
/**
   \ru проверка, достаточно точек на экране выбрано для построения кривой

*/
//-----------------------------------------------------------------------------
bool MainWindowHandler::IsSufficientNum()
{
  return ( points.size() == geomCreator.numExpectedPoits );
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
  geomCreator.numExpectedPoits = 1;
  geomCreator.creator = new PointCreator();
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
  geomCreator.numExpectedPoits = 2;
  geomCreator.creator = new LineCreator();
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
  geomCreator.numExpectedPoits = 3;
  geomCreator.creator = new EllipseCreator();
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
  geomCreator.numExpectedPoits = 2;
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
void MainWindowHandler::CreateCurve()
{
  std::vector<Point> currentPoints;
  DisplayedCurve * curve = new DisplayedCurve;

  std::shared_ptr<GeometricPrimitive> primitive = geomCreator.creator->Create( points );
  curve->primitive = primitive;
  curve->referencedPoints = points;
  displayedCurves.push_back( curve );
  primitive->GetAsPolyLine( currentPoints, 0.001 );
  geomPolylines.push_back( currentPoints );
  printChart.AddFigure( curve );
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

  if ( state == StateCreateCurve || state == StateCreatePolyline )
  {
    QPointF currentPoint = chart->mapToValue( event->pos() );
    AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
    if ( IsSufficientNum() )
    {
      CreateCurve();
      points.clear();
    }
  }
  else
    if ( state == StateExpectAction )
    {
      StateExpect ( event );
    }
}


//-----------------------------------------------------------------------------
/**
  обработка ожидания действия:
  если можно селектировать кривую - селектируем кривую
  если нельзя - убираем все селкции
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::StateExpect( QMouseEvent *event )
{
  QPointF currentPoint = chart->mapToValue( QPointF(event->pos().x(), event->pos().y() - 30) );
  int selectSeries = selector.GetCurve( Point (currentPoint.x(), currentPoint.y()) );

  if ( selectSeries != -1 ) {
    isSelected.push_back(selectSeries);
    chart->removeAllSeries();

    for ( int i = 0; i < displayedCurves.size(); i++ )
    {
      if (std::find (isSelected.begin(), isSelected.end(), i ) == isSelected.end() )
        printChart.AddFigure( displayedCurves[i], selectingColor );
      else
        printChart.AddFigure( displayedCurves[i], currentColor );
    }
  }

  if ( selectSeries == -1 )
  {
    chart->removeAllSeries();
    for ( int j = 0; j < displayedCurves.size(); j++ )
      printChart.AddFigure( displayedCurves[j], selectingColor );
    isSelected.clear();
  }
  state = StateExpectAction;
}


//-----------------------------------------------------------------------------
/**
  функции изменения цвета кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::ChangeColor( QColor color )
{
  for ( int i = 0; i < isSelected.size(); i++ )
   printChart.AddFigure( displayedCurves[isSelected[i]], color );
}


//-----------------------------------------------------------------------------
/**
  функция удаления кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::DeleteCurve()
{
  std::sort( isSelected.begin(), isSelected.end(), std::greater<int>() );
  for ( int i = 0; i < isSelected.size(); i++ )
  {
    displayedCurves.erase( displayedCurves.begin() + isSelected[i] );
  }
  chart->removeAllSeries();
  for ( int i = 0; i < displayedCurves.size(); i++ )
    printChart.AddFigure( displayedCurves[i], selectingColor );

  isSelected.clear();
}


//-----------------------------------------------------------------------------
/**
  функции обработки события изменения размера окна
*/
//-----------------------------------------------------------------------------
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
  int i = QInputDialog::getInt( 0, ("Polyline Points"), ("amount of points"), 25, 0, 100, 1, &ok );
  if (ok)
    geomCreator.numExpectedPoits = i;
  state = StateCreatePolyline;
  geomCreator.creator = new GeomPolylineCreator();
}
