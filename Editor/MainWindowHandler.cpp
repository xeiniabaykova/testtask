#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "Editor/CircleCreator.h"
#include "FileIO.h"
#include <QtWidgets/QDesktopWidget>
#include <QInputDialog>
#include <QtCharts/QLineSeries>
#include "GeomPolylineCreator.h"
#include <functional>



//-----------------------------------------------------------------------------
/**
  \ru Конструктор MainWindowHandler
*/
//-----------------------------------------------------------------------------
MainWindowHandler::MainWindowHandler (QChart * chart):
  chart         ( chart             ),
  printChart    ( chart             ),
  points        ( 0                 ),
  geomCreator   ( 0, nullptr        ),
  state         ( StateExpectAction )
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
bool MainWindowHandler::IsSufficientNum() const
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
 // geomCreator.creator = new PointCreator();
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
  geomCreator.creator = new CircleCreator();
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
  double accuracy = 0.01;
  std::vector<Point> currentPoints;
  std::shared_ptr<GeometricPrimitive> primitive = geomCreator.creator->Create( points );
  primitive->GetAsPolyLine( currentPoints, accuracy );
  std::shared_ptr<DisplayedCurve> curve = std::make_shared<DisplayedCurve>( chart, points, currentPoints );
  displayedCurves.push_back( curve );
  printChart.AddFigure( curve );
}

void MainWindowHandler::StopCreateCurve()
{
  if ( state == StateCreatePolyline )
  {
    CreateCurve();
    points.clear();
  }
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
  if ( event->buttons() == Qt::RightButton )
    return;
  if ( event->buttons() == Qt::MiddleButton )
    state = StopCreatePolyline;


  QRect rec = QApplication::desktop()->screenGeometry();
  double height = rec.height();
  double width = rec.width();
  chart->resize( width, height );
  CreateEmptySeries();
  if ( state == StateCreateCurve  || state == StateCreatePolyline )
  {
    QPointF currentPoint = chart->mapToValue( QPointF(event->x(), event->y() - 30) );
    AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
    printChart.AddReferencedPoint( Point(currentPoint.x(), currentPoint.y()) );
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
  else
    if (state == StopCreatePolyline)
    {
      CreateCurve();
      points.clear();
      CreatePolyline();
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
  for ( int i = 0; i < displayedCurves.size(); i++ )
    displayedCurves[i]->ModifySelectionStatus( Point(currentPoint.x(), currentPoint.y()) );
  state = StateExpectAction;
}


//-----------------------------------------------------------------------------
/**
  функции изменения цвета кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::ChangeColor( QColor color )
{
  for ( int i = 0; i < displayedCurves.size(); i++ )
   if ( displayedCurves[i]->GetSelectionStatus() )
     displayedCurves[i]->SetColor( color );
}


//-----------------------------------------------------------------------------
/**
  функция удаления кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::DeleteCurve()
{
  for ( int i = 0; i < displayedCurves.size(); i++ )
    if ( displayedCurves[i]->GetSelectionStatus() )
       displayedCurves.erase( displayedCurves.begin() +i );

  state = StateExpectAction;
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


//-----------------------------------------------------------------------------
/**
  функции обработки события создания полилнии
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreatePolyline()
{
  state = StateCreatePolyline;
  geomCreator.creator = new GeomPolylineCreator();
  geomCreator.numExpectedPoits = -1;
}


//-----------------------------------------------------------------------------
/**
  КОСТЫЛЬ! для верной работы функции maptovalue
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateEmptySeries()
{
  QLineSeries *series = new QLineSeries;
  *series<<QPointF( 0, 0 ) << QPointF( 10, 10 );
  series->setColor( QColor(255,255,255) );
  chart->addSeries( series );
}


//-----------------------------------------------------------------------------
/**
   обработка события очистки экрана
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::ClearScreen()
{
   chart->removeAllSeries();
   displayedCurves.clear();
   state = StateExpectAction;
}



