#include "CreatorHandler.h"
#include "Math/GeomPolyline.h"
#include "Math/Line.h"
#include "Math/NurbsCurve.h"

#include "Math/Ellipse.h"

namespace Editor {

CreatorHandler::CreatorHandler( int theNumExpectedPoits, TypeCurve theType ):
  numExpectedPoits ( theNumExpectedPoits ),
  type             ( theType )
{
}

//-----------------------------------------------------------------------------
/**
  Добавление точки с экрана в массив точек.
*/
//---
void CreatorHandler::AddPointFromScreen( Math::Point point )
{
  points.push_back( point );
}


//-----------------------------------------------------------------------------
/**
   Проверка, достаточно точек на экране выбрано для построения кривой.
*/
//---
bool CreatorHandler::IsSufficientNum() const
{
  return ( points.size() == numExpectedPoits );
}


//-----------------------------------------------------------------------------
/**
   Создает кривую по точкам в зависимости от типа.
*/
//---
std::shared_ptr<Math::Curve> CreatorHandler::Create()
{
  switch( type ){
  case CreateLine:
  {
    return std::make_shared<Math::Line>( points );
    break;
  }

  case CreateEllipse:
  {
    return std::make_shared<Math::Ellipse>( points );
    break;
  }   
  case CreatePolyline:
  {
    return std::make_shared<Math::GeomPolyline>( points );
     break;
  }

  case CreateNURBS:
  {
    std::vector<double> weights;
    for ( int i = 0; i < points.size(); i++ )
      weights.push_back(1.0 );

    int degree = 3;
    std::vector<double> nodes;

    bool isClosed = false;
    return std::make_shared<Math::NurbsCurve>( points, weights, nodes, isClosed, degree );
      break;
  }  
  }
  return nullptr;
}


//-----------------------------------------------------------------------------
/**
    Очистить массив точек, полученных с экрана.
*/
//---
void CreatorHandler::ClearPoints()
{
  points.clear();
}
}
