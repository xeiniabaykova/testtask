#include "MainWindowHandler.h"
#include "FileIO.h"
#include <QtWidgets/QDesktopWidget>
#include <QInputDialog>
#include <QtCharts/QLineSeries>
#include <functional>



//-----------------------------------------------------------------------------
/**
  \ru Конструктор MainWindowHandler
*/
//-----------------------------------------------------------------------------
MainWindowHandler::MainWindowHandler (QChart * chart):
  chart         ( chart             ),
  state         ( StateExpectAction ),
  precision     ( 0.01 ),
  selectedColor ( 10, 50, 255 )
{
  geomCreator = std::make_shared<CreatorHandler>();
  CreateChart();
}

void MainWindowHandler::CreateLine()
{
  state = StateCreateCurve;
  geomCreator->AddSufficientNum( 2 );
  geomCreator->type = CreatorHandler::CreateLine;
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
  geomCreator->type = CreatorHandler::CreateEllipse;
  geomCreator->AddSufficientNum( 3 );
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
  geomCreator->type = CreatorHandler::CreateEllipse;
  geomCreator->AddSufficientNum( 2 );
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - 2
  \ru создается объект для создания геометрического представления nurbs

*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreateNurbs()
{
  geomCreator->type = CreatorHandler::CreateNURBS;
// state = CreateCurve;
}


//-----------------------------------------------------------------------------
/**
  функции обработки события создания полилнии
*/
//-----------------------------------------------------------------------------
void MainWindowHandler::CreatePolyline()
{
  state = StateCreatePolyline;
  geomCreator->type = CreatorHandler::CreatePolyline;
  geomCreator->AddSufficientNum( -1 );
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
  std::shared_ptr<DisplayedObject> curve = std::make_shared<DisplayedObject>( primitive, axisX, axisY );
  curve->addCurveToChart( chart, precision );
  displayedCurves.push_back( curve );
}

void MainWindowHandler::StopCreateCurve()
{
  if ( state == StateCreatePolyline )
  {
    CreateCurve();
    seriesReferenced->clear();
   // geomCreator;
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
    CreateRefPoint( Point(currentPoint.x(), currentPoint.y()) );
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
    displayedCurves[i]->ModifySelectionStatus( Point(currentPoint.x(), currentPoint.y()), precision, selectedColor );
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
       displayedCurves.erase( displayedCurves.begin() + i );

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
   state = StateExpectAction;
}


void MainWindowHandler::CreateChart()
{
    axisX = new QValueAxis;
    axisX->setRange( -5, 3 );
    chart->addAxis( axisX, Qt::AlignBottom );

    axisY = new QValueAxis;
    axisY->setRange( -5, 5 );
    chart->addAxis( axisY, Qt::AlignLeft );
    chart->legend()->setVisible(false);

    QLineSeries *series = new QLineSeries;
    *series<< QPointF( -5, -5 ) << QPointF( 5, 5);
    series->setColor( QColor(255,255,255) );
    chart->addSeries( series );

    series->attachAxis( axisX );
    series->attachAxis( axisY );

    seriesReferenced = new QScatterSeries();
    seriesReferenced->setColor( QColor(0, 17, 17) );
    chart->addSeries( seriesReferenced );
    seriesReferenced->attachAxis( axisX );
    seriesReferenced->attachAxis( axisY );

   // chart->axisX()->setVisible( false );
  //  chart->axisY()->setVisible( false );
}


void MainWindowHandler::CreateRefPoint( Point point )
{
  *seriesReferenced << QPointF( point.GetX(), point.GetY() );
  seriesReferenced->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesReferenced->setMarkerSize( 15.0 );


}
