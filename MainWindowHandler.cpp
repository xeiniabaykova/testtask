#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "FileIO.h"

MainWindowHandler::MainWindowHandler (QChart * chart):
  chart(chart){}

void MainWindowHandler::AddPointFromScreen( QPoint point ) {
  pointsFromScreen.points.push_back( point );
}

void MainWindowHandler::AddSufficientNum ( int num ) {
  sufficient = num;
}

bool MainWindowHandler::IsSufficientNum(){
  return ( pointsFromScreen.points.size() == sufficient );
}

void MainWindowHandler::CreatePoint() {

  sufficient = 1;
  creator = new PointCreator();
}

void MainWindowHandler::CreateLine() {

  sufficient = 2;
  creator = new LineCreator();
}

void MainWindowHandler::CreateEllipse() {

  sufficient = 3;
  creator = new EllipseCreator();
}

void MainWindowHandler::CreateNurbs() {

}

void MainWindowHandler::LoadFile() {

  FileIO open;
  open.Open();
}

void MainWindowHandler::SaveFile() {

    FileIO save;
    save.Save();
}

 void MainWindowHandler::CreateCurve() {
     creator->Create( chart,pointsFromScreen.points );

 }

 void MainWindowHandler:MouseEvent(){

  if ( !windowHandler.IsSufficientNum() )
      {
        windowHandler.AddPointFromScreen( event->pos() );
      } else {
       windowHandler.CreateCurve();
 }
