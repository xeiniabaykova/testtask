#include "CreatorHandler.h"
#include "Math/GeomPolyline.h"
#include "Math/Line.h"

#include "Math/Ellipse.h"


//-----------------------------------------------------------------------------
/**
  \ru добавление точки с экрана в массив точек
*/
//-----------------------------------------------------------------------------
void CreatorHandler::AddPointFromScreen( Point point )
{
  points.push_back( point );
}


//-----------------------------------------------------------------------------
/**
 \ru  возвращает количество точек, необходимое для отрисовки текущего геометрического примитива
*/
//-----------------------------------------------------------------------------
void CreatorHandler::AddSufficientNum ( int num )
{
  numExpectedPoits = num;
}


//-----------------------------------------------------------------------------
/**
   \ru проверка, достаточно точек на экране выбрано для построения кривой
*/
//-----------------------------------------------------------------------------
bool CreatorHandler::IsSufficientNum() const
{
  return ( points.size() == numExpectedPoits );
}


//-----------------------------------------------------------------------------
/**
   \ru создает кривую по точкам
*/
//-----------------------------------------------------------------------------
std::shared_ptr<C2Curve> CreatorHandler::Create()
{
  switch( type ){
  case CreateLine:{
    return std::make_shared<Line>( points );
  }
    break;
  case CreateEllipse:{
    return std::make_shared<Ellipse>( points );
  }
    break;
  case CreatePolyline:{
    return std::make_shared<GeomPolyline>( points );
  }
    break;
  case CreateNURBS:
    break;
  }
  return 0;
}


//-----------------------------------------------------------------------------
/**
   \ru возвратить опорные точки
*/
//-----------------------------------------------------------------------------
const std::vector<Point>& CreatorHandler::RefPoints() const
{
  return points;
}


//-----------------------------------------------------------------------------
/**
   \ru очистить массив точек, полученных с экрана
*/
//-----------------------------------------------------------------------------
void CreatorHandler::ClearPoints()
{
  points.clear();
}
