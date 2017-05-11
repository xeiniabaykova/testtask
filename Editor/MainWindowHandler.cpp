#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
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
  state         ( StateExpectAction )
{
}

void MainWindowHandler::CreateLine()
{
  state = StateCreateCurve;
  GeometricPrimitiveCreator * creator = new LineCreator();
  geomCreator = new CreatorHandler( 2, creator );
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
  GeometricPrimitiveCreator * creator = new EllipseCreator();
  geomCreator = new CreatorHandler( 3, creator );
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
  GeometricPrimitiveCreator * creator = new EllipseCreator();
  geomCreator = new CreatorHandler( 2, creator );
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
  std::vector<Point> currentPolylinePoints;
  std::shared_ptr<C2Curve> primitive = geomCreator->Create();
  primitive->GetAsPolyLine( currentPolylinePoints, accuracy );
  std::vector<Point> currentRefPoints = geomCreator->RefPoints();
  std::shared_ptr<DisplayedCurve> curve = std::make_shared<DisplayedCurve>( chart, currentRefPoints, currentPolylinePoints );
  displayedCurves.push_back( curve );
  printChart.AddFigure( curve );
}

void MainWindowHandler::StopCreateCurve()
{
  if ( state == StateCreatePolyline )
  {
    CreateCurve();
    delete geomCreator;
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
    geomCreator->AddPointFromScreen( Point(currentPoint.x(), currentPoint.y()) );
    printChart.AddReferencedPoint( Point(currentPoint.x(), currentPoint.y()) );
    if ( geomCreator->IsSufficientNum() )
    {
      CreateCurve();
      geomCreator->ClearPoints();
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
      geomCreator->ClearPoints();
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
     displayedCurves[i]->SetColorUnselectedCurve( color );
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
  GeometricPrimitiveCreator * creator = new GeomPolylineCreator();
  geomCreator = new CreatorHandler( -1, creator );
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
   displayedCurves.clear();
  // chart->removeAllSeries();
   state = StateExpectAction;
}



