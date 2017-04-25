#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "FileIO.h"
#include <QtWidgets/QMessageBox>



//-----------------------------------------------------------------------------
/**
  \ru Конструктор MainWindowHandler
*/
//-----------------------------------------------------------------------------
MainWindowHandler::MainWindowHandler (QChart * chart):
  chart( chart ),
  printChart( chart ),
  points(0),
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
void MainWindowHandler::CreateCurve()
{
  DisplayedFigure currentFugure( GeomCreator.creator->Create(points) );
  printChart.AddFigure( currentFugure.GetPoints() );
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
  if (state == StateCreateCurve) {
     if ( !IsSufficientNum() )
     {
       AddPointFromScreen( Point(event->pos().x(), event->pos().y()) );
       PrintCharacteristicPoint ( Point(event->pos().x(), event->pos().y()) );
     } else {
       AddPointFromScreen( Point(event->pos().x(), event->pos().y()) );
       PrintCharacteristicPoint ( Point(event->pos().x(), event->pos().y()) );
       CreateCurve();
     }
  }
}


void MainWindowHandler::PrintCharacteristicPoint( Point point )
{
  PointCreator *creator = new PointCreator();
  std::vector<Point> currentPoint;
  currentPoint.push_back( point );
  DisplayedFigure currentFugure( creator->Create(currentPoint) );
  printChart.AddFigure( currentFugure.GetPoints() );
}
