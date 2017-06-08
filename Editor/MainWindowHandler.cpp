#include "MainWindowHandler.h"
#include "FileIO.h"

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QInputDialog>
#include <QtCharts/QLineSeries>
#include <functional>
#include "CommonConstantsEditor.h"


namespace Editor {
//-----------------------------------------------------------------------------
/**
  Конструктор MainWindowHandler
*/
//---
MainWindowHandler::MainWindowHandler (QChart * chart):
  chart         ( chart             ),
  state         ( StateExpectAction ),
  selectedColor ( 10, 50, 255 )
{

  CreateChart();
}


//-----------------------------------------------------------------------------
/**
  Задается необходимое количество точек - 2
  Создается объект для создания геометрического представления отрезка.

*/
//--
void MainWindowHandler::CreateLine()
{
  state = StateCreateCurve;
  geomCreator = std::make_shared<CreatorHandler>( 2, CreatorHandler::CreateLine );
}


//-----------------------------------------------------------------------------
/**
  Задается необходимое количество точек - 3
  Создается объект для создания геометрического представления эллипса

*/
//--
void MainWindowHandler::CreateEllipse()
{
  geomCreator = std::make_shared<CreatorHandler>( 3, CreatorHandler::CreateEllipse );
  state = StateCreateCurve;
}


//-----------------------------------------------------------------------------
/**
  Задается необходимое количество точек - 2
  Создается объект для создания геометрического представления окружности

*/
//---
void MainWindowHandler::CreateCircle()
{
  state = StateCreateCurve;
  geomCreator = std::make_shared<CreatorHandler>( 2, CreatorHandler::CreateEllipse );
}


//-----------------------------------------------------------------------------
/**
  \ru задается необходимое количество точек - -1
  \ru создается объект для создания геометрического представления nurbs

*/
//---
void MainWindowHandler::CreateNurbs()
{
  state = StateCreateLine;
  geomCreator = std::make_shared<CreatorHandler>( -1, CreatorHandler::CreateNURBS );
}


//-----------------------------------------------------------------------------
/**
  функции обработки события создания полилнии
*/
//---
void MainWindowHandler::CreatePolyline()
{
  state = StateCreateLine;
  geomCreator = std::make_shared<CreatorHandler>( -1, CreatorHandler::CreatePolyline );
}


//-----------------------------------------------------------------------------
/**
  Создается объект для открытия файла.
  Запускается окно открытия файла.
*/
//---
void MainWindowHandler::LoadFile()
{
  FileIO open;
  open.Open();
}


//-----------------------------------------------------------------------------
/**
  Создается объект для сохранения файла.
  Запускается окно сохранения файла.
*/
//---
void MainWindowHandler::SaveFile( const std::vector<std::shared_ptr<DisplayedObject>>& savedObj )
{
  FileIO save;
  //save.Save();
}


//-----------------------------------------------------------------------------
/**
  Вызывается метод создания кривой по точкам.
  Обнуляется массив точек полученных с экрана.
*/
//---
void MainWindowHandler::CreateCurve()
{
  std::shared_ptr<Math::Curve> primitive = geomCreator->Create();
  std::shared_ptr<DisplayedObject> curve = std::make_shared<DisplayedObject>( primitive, axisX, axisY );
  curve->addCurveToChart( chart );
  displayedCurves.push_back( curve );
  tempSeriesReferenced->clear();
}

void MainWindowHandler::StopCreateCurve()
{
  if ( state == StateCreateLine )
  {
    CreateCurve();
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
  if ( state == StateCreateCurve  || state == StateCreateLine )
  {
    QPointF currentPoint = chart->mapToValue( QPointF(event->x(), event->y() - 30) );
    geomCreator->AddPointFromScreen( Math::Point(currentPoint.x(), currentPoint.y()) );
    CreateRefPoint( Math::Point(currentPoint.x(), currentPoint.y()) );
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
    displayedCurves[i]->ModifySelectionStatus( Math::Point(currentPoint.x(), currentPoint.y()), CommonConstantsEditor::PRECISION_SELECT, selectedColor );
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
void MainWindowHandler::ResizeEvent( QResizeEvent* )
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
   Обработка события очистки экрана.
*/
//---
void MainWindowHandler::ClearScreen()
{ 
   displayedCurves.clear();
   state = StateExpectAction;
}


//-----------------------------------------------------------------------------
/**
   Обработка события очистки экрана.
*/
//---
void MainWindowHandler::CreateChart()
{
  axisX = new QValueAxis;
  axisX->setRange( 0, 10 );
  chart->addAxis( axisX, Qt::AlignBottom );

  axisY = new QValueAxis;
  axisY->setRange( 0, 10 );
  chart->addAxis( axisY, Qt::AlignLeft );
  chart->legend()->setVisible(false);

  QLineSeries *series = new QLineSeries;
  *series<< QPointF( 0, 0 ) << QPointF( 10, 10);
  series->setColor( QColor(255,255,255) );
  chart->addSeries( series );

  series->attachAxis( axisX );
  series->attachAxis( axisY );

  tempSeriesReferenced = new QScatterSeries();
  tempSeriesReferenced->setColor( QColor(0, 17, 17) );
  chart->addSeries( tempSeriesReferenced );
  tempSeriesReferenced->attachAxis( axisX );
  tempSeriesReferenced->attachAxis( axisY );
  tempSeriesReferenced->clear();
  chart->axisX()->setVisible( false );
  chart->axisY()->setVisible( false );
}


//-----------------------------------------------------------------------------
/**
   Отобразить точку, выбранную пользователем.
*/
//---
void MainWindowHandler::CreateRefPoint( Math::Point point )
{
  *tempSeriesReferenced << QPointF( point.GetX(), point.GetY() );
  tempSeriesReferenced->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  tempSeriesReferenced->setMarkerSize( 15.0 );
}


}
