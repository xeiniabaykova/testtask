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
#include "Math/Intersector.h"
#include "Math/Curve.h"


namespace Editor {
//-----------------------------------------------------------------------------
// Конструктор MainWindowHandler. Устанавилвается цвет селектирования кривой,
// состояние формы, окно для рисования графика.
// ---
MainWindowHandler::MainWindowHandler(QChart * chart):
  chart         ( chart             ),
  state         ( StateExpectAction ),
  selectedColor ( 10, 50, 255 )
{
  CreateChart();
}


//-----------------------------------------------------------------------------
//  Задается необходимое количество точек - 2
//  Создается объект для создания геометрического представления отрезка.
// ---
void MainWindowHandler::CreateLine()
{
  state = StateCreateCurve;
  geomCreator = std::make_shared<CreatorHandler>( 2, CreatorHandler::CreateLine );
}


//-----------------------------------------------------------------------------
//  Задается необходимое количество точек - 3
//  Создается объект для создания геометрического представления эллипса
// ---
void MainWindowHandler::CreateEllipse()
{
  geomCreator = std::make_shared<CreatorHandler>( 3, CreatorHandler::CreateEllipse );
  state = StateCreateCurve;
}


//-----------------------------------------------------------------------------
//  Задается необходимое количество точек - 2
//  Создается объект для создания геометрического представления окружности
// ---
void MainWindowHandler::CreateCircle()
{
  state = StateCreateCurve;
  geomCreator = std::make_shared<CreatorHandler>( 2, CreatorHandler::CreateEllipse );
}


//-----------------------------------------------------------------------------
// Задается необходимое количество точек - -1
// Создается объект для создания геометрического представления nurbs
// ---
void MainWindowHandler::CreateNurbs()
{
  state = StateCreateNURBS;
  geomCreator = std::make_shared<CreatorHandler>( -1, CreatorHandler::CreateNURBS );
}


//-----------------------------------------------------------------------------
// Функции обработки события создания полилнии
// ---
void MainWindowHandler::CreatePolyline()
{
  state = StateCreateLine;
  geomCreator = std::make_shared<CreatorHandler>( -1, CreatorHandler::CreatePolyline );
}


//-----------------------------------------------------------------------------
//  Создается объект для открытия файла.
//  Запускается окно открытия файла.
// ---
void MainWindowHandler::LoadFile()
{
  FileIO open;
  std::vector<std::shared_ptr<Math::Curve>> inputObj;
  open.Open( inputObj );
  for ( size_t i = 0; i < inputObj.size(); i++ )
  {
    std::shared_ptr<DisplayedObject> curve = std::make_shared<DisplayedObject>( inputObj[i] );
    curve->AddCurveToChart( chart );
    displayedCurves.push_back( curve );
  }
}


//-----------------------------------------------------------------------------
// Создается объект для сохранения файла.
// Запускается окно сохранения файла.
// ---
void MainWindowHandler::SaveFile()
{
  std::vector<std::shared_ptr<Math::Curve>> savedCurves;

  for ( size_t i = 0; i < displayedCurves.size(); i++ )
  {
    std::shared_ptr<Math::Curve> curve = displayedCurves[i]->GetPrimitive();
    savedCurves.push_back( curve  );
  }

  FileIO save;
  save.Save( savedCurves );
}


//-----------------------------------------------------------------------------
//  Вызывается метод создания кривой по точкам.
//  Обнуляется массив точек полученных с экрана.
// ---
void MainWindowHandler::CreateCurve()
{
  std::shared_ptr<Math::Curve> primitive = geomCreator->Create();
  if ( primitive == nullptr )
    return;
  std::shared_ptr<DisplayedObject> curve = std::make_shared<DisplayedObject>( primitive );

  curve->AddCurveToChart( chart );
  displayedCurves.push_back( curve );
  tempSeriesReferenced->clear();
}


//-----------------------------------------------------------------------------
//  Остановка создания кривых.Создается кривая, режим переключается в режим ожидания действия.
//  Обнуляется массив точек полученных с экрана.
// ---
void MainWindowHandler::StopCreateCurve()
{
  if ( state == StateCreateLine || state == StateCreateNURBS )
  {
    CreateCurve();
  }
  state = StateExpectAction;
}


//-----------------------------------------------------------------------------
//  Обработка события клика мышкой:
// если количество точек достаточно для создания крвой, то она создается,
// если недостаточно, точка добавляется в массив.
// ---
void MainWindowHandler::MouseEvent( QMouseEvent *event )
{
  if ( event->buttons() == Qt::RightButton )
    return;

  if ( event->buttons() == Qt::MiddleButton )
  {
    if ( state == StateCreateLine )
      state = StopCreatePolyline;
    if (state == StateCreateNURBS)
      state = StopCreateNurbs;

  }

  QRect rec = QApplication::desktop()->screenGeometry();
  double height = rec.height();
  double width = rec.width();
  chart->resize( width, height );
  CreateEmptySeries();
  if ( state == StateCreateCurve  || state == StateCreateLine || state == StateCreateNURBS )
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
    if ( state == StopCreatePolyline )
    {
      CreateCurve();
      geomCreator->ClearPoints();
      CreatePolyline();
    }
  if ( state == StopCreateNurbs )
  {
    CreateCurve();
    geomCreator->ClearPoints();
    CreateNurbs();
  }
}


//-----------------------------------------------------------------------------
//  Обработка ожидания действия:
//  если можно селектировать кривую - селектируем кривую.
//  если кривая селектированна, то убираем селелекцию.
//-----------------------------------------------------------------------------
void MainWindowHandler::StateExpect( QMouseEvent *event )
{
  QPointF currentPoint = chart->mapToValue( QPointF(event->pos().x(), event->pos().y() - 30) );
  for ( size_t i = 0; i < displayedCurves.size(); i++ )
    displayedCurves[i]->ModifySelectionStatus( Math::Point(currentPoint.x(), currentPoint.y()), CommonConstantsEditor::PRECISION_SELECT, selectedColor );
  state = StateExpectAction;
}


//-----------------------------------------------------------------------------
//  Изменить цвет селектированной кривой
//---
void MainWindowHandler::ChangeColor( QColor color )
{
  for ( size_t i = 0; i < displayedCurves.size(); i++ )
    if ( displayedCurves[i]->GetSelectionStatus() )
      displayedCurves[i]->SetColorUnselectedCurve( color );
}


//-----------------------------------------------------------------------------
//  Удалить селектированные кривые.
//---
void MainWindowHandler::DeleteCurve()
{
  for ( size_t i = 0; i < displayedCurves.size(); i++ )
    if ( displayedCurves[i]->GetSelectionStatus() )
       displayedCurves.erase( displayedCurves.begin() + i );

  state = StateExpectAction;
}


//-----------------------------------------------------------------------------
//  Обработать изменение размера окна.
//---
void MainWindowHandler::ResizeEvent( QResizeEvent* )
{
  QRect rec = QApplication::desktop()->screenGeometry();
  double height = rec.height();
  double width = rec.width();
  chart->resize( width, height );
}



//-----------------------------------------------------------------------------
//  КОСТЫЛЬ! для верной работы функции maptovalue
//---
void MainWindowHandler::CreateEmptySeries()
{
  QLineSeries *series = new QLineSeries;
  *series<<QPointF( 0, 0 ) << QPointF( 10, 10 );
  series->setColor( QColor(255,255,255) );
  chart->addSeries( series );
}


//-----------------------------------------------------------------------------
//   Обработать событие очистки экрана.
//---
void MainWindowHandler::ClearScreen()
{ 
   displayedCurves.clear();
   tempSeriesReferenced->clear();
   state = StateExpectAction;
}


//-----------------------------------------------------------------------------
//  Обработать очистку экрана.
//---
void MainWindowHandler::CreateChart()
{
  QValueAxis *axisX = new QValueAxis;
  axisX->setRange( 0, 10 );
  chart->addAxis( axisX, Qt::AlignBottom );

  QValueAxis *axisY = new QValueAxis;
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
//   Отобразить точку, выбранную пользователем.
//---
void MainWindowHandler::CreateRefPoint( Math::Point point )
{
  *tempSeriesReferenced << QPointF( point.GetX(), point.GetY() );
  tempSeriesReferenced->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  tempSeriesReferenced->setMarkerSize( 15.0 );
}


//-----------------------------------------------------------------------------
//    Найти точки пересечений кривых.
//---
void MainWindowHandler::FindIntersections()
{
  std::vector<Math::Curve*> curves;
  for ( size_t i = 0; i < displayedCurves.size(); i++ )
  {
    if ( displayedCurves[i]->GetSelectionStatus() )
      curves.push_back( displayedCurves[i]->GetPrimitive().get() );
  }
  std::map<std::pair<const Math::Curve*, const Math::Curve*>,
          std::vector<std::pair<double, double>>> intersectionPoints;
  Math::Intersect( curves,intersectionPoints );
  for ( auto it = intersectionPoints.begin(); it != intersectionPoints.end(); it++ )
  {
      std::vector<std::pair<double, double>> points = it->second;
      for ( size_t i = 0; i < points.size(); i++ )
      {
          Math::Point point;
          it->first.first->GetPoint( points[i].first,point );
          CreateRefPoint( point );
      }
  }
}


//-----------------------------------------------------------------------------
//    Удалить опорные точки.
//---
void MainWindowHandler::DelRefPoints()
{
  for ( size_t i = 0; i < displayedCurves.size(); i++ )
  {
     displayedCurves[i]->DelRefSeries();
  }
}

}
