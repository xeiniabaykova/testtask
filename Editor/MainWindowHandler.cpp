#include "MainWindowHandler.h"
#include "PointCreator.h"
#include "LineCreator.h"
#include "EllipseCreator.h"
#include "FileIO.h"

MainWindowHandler::MainWindowHandler (QChart * chart):
  chart(chart){}

void MainWindowHandler::AddPointFromScreen( QPoint point ) {
  points.push_back( point );
}

void MainWindowHandler::AddSufficientNum ( int num ) {
  GeomCreator.sufficient = num;
}

bool MainWindowHandler::IsSufficientNum(){
  return ( points.size() == GeomCreator.sufficient );
}

void MainWindowHandler::CreatePoint() {

  GeomCreator.sufficient = 1;
  GeomCreator.creator = new PointCreator();
}

void MainWindowHandler::CreateLine() {

  GeomCreator.sufficient = 2;
  GeomCreator.creator = new LineCreator();
}

void MainWindowHandler::CreateEllipse() {

  GeomCreator.sufficient = 3;
  GeomCreator.creator = new EllipseCreator();
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
  GeomCreator.creator->Create( chart,points );

}

void MainWindowHandler::MouseEvent( QMouseEvent *event ) {

 if ( !IsSufficientNum() )
 {
   AddPointFromScreen( event->pos() );
 } else {
   CreateCurve();
 }
}
