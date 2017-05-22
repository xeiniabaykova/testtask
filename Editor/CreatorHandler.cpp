#include "CreatorHandler.h"
#include "Math/GeomPolyline.h"
#include "Math/Line.h"
#include "Math/NurbsCurve.h"

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
  case CreateNURBS:{
    std::vector<double> weights;
    for ( int i = 0; i < points.size(); i++ )
      weights.push_back(1.0 );

    double degree = 3;
    std::vector<double> nodes;
    for ( int i = 0; i < degree + 1; i++ )
      nodes.push_back( 0.0 );
    int k = 0;
    for ( int i = degree + 1; i < points.size() - degree -1; i++ )
    {
      nodes.push_back( (i - 2) );
      k++;
    }

    for ( int i = points.size() - degree -1; i < points.size(); i++ )
      nodes.push_back( (points.size() - 2*(degree +1) +1));
    bool isClosed = false;

    return std::make_shared<NurbsCurve>( points, weights, nodes, isClosed, degree );
  }
    break;
  }
  return 0;
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
