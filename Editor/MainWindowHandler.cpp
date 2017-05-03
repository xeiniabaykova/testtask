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
  selector      ( geomPolylines     ),
  geomCreator   ( 0, nullptr        ),
  state         ( StateExpectAction ),
  normalColor   ( 51, 0, 51         ),
  selectedColor ( 190, 0, 21        ),
  accuracy      ( 0.001             )
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
  std::vector<Point> currentPoints;
  std::shared_ptr<DisplayedCurve> curve = std::make_shared<DisplayedCurve>();

  std::shared_ptr<GeometricPrimitive> primitive = geomCreator.creator->Create( points );
  curve->primitive = primitive;
  curve->referencedPoints = points;
  displayedCurves.push_back( curve );
  primitive->GetAsPolyLine( currentPoints, accuracy );
  geomPolylines.push_back( currentPoints );
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
  int selectedSeries = selector.GetCurve( Point (currentPoint.x(), currentPoint.y()) );
  // если selectedSeries == -1, то ни одна линия не селектированна
  if ( selectedSeries != -1 )
  {
    DoubleSelectionRemoved( selectedSeries );
    chart->removeAllSeries();
    for ( int i = 0; i < displayedCurves.size(); i++ )
    {
      if ( std::find(selectedIndexes.begin(), selectedIndexes.end(), i ) == selectedIndexes.end() )
        printChart.AddFigure( displayedCurves[i],  normalColor );
      else
        printChart.AddFigure( displayedCurves[i], selectedColor );
    }
  }
  state = StateExpectAction;
}

void MainWindowHandler::DoubleSelectionRemoved( int indexSelectedCurve )
{
  auto it = std::find( selectedIndexes.begin(), selectedIndexes.end(), indexSelectedCurve );
    if ( it != selectedIndexes.end() )
      selectedIndexes.erase( it );
    else
      selectedIndexes.push_back( indexSelectedCurve );

}

//-----------------------------------------------------------------------------
/**
  функции изменения цвета кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::ChangeColor( QColor color )
{
  for ( int i = 0; i < selectedIndexes.size(); i++ )
   printChart.AddFigure( displayedCurves[selectedIndexes[i]], color );
  selectedIndexes.clear();
}


//-----------------------------------------------------------------------------
/**
  функция удаления кривой
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::DeleteCurve()
{
  std::sort( selectedIndexes.begin(), selectedIndexes.end(), std::greater<int>() );
  for ( int i = 0; i < selectedIndexes.size(); i++ )
  {
    displayedCurves.erase( displayedCurves.begin() + selectedIndexes[i] );
    geomPolylines.erase( geomPolylines.begin() + selectedIndexes[i] );
  }
  chart->removeAllSeries();
  for ( int i = 0; i < displayedCurves.size(); i++ )
    printChart.AddFigure( displayedCurves[i], normalColor );

  selectedIndexes.clear();
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
   selectedIndexes.clear();
   displayedCurves.clear();
   geomPolylines.clear();
   state = StateExpectAction;
}
