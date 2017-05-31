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
void CreatorHandler::AddPointFromScreen( Math::Point point )
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
std::shared_ptr<Math::Curve> CreatorHandler::Create()
{
  switch( type ){
  case CreateLine:{
    return std::make_shared<Math::Line>( points );
  }
    break;
  case CreateEllipse:{
    return std::make_shared<Math::Ellipse>( points );
  }
    break;
  case CreatePolyline:{

    return std::make_shared<Math::GeomPolyline>( points );
  }
    break;
  case CreateNURBS:{
    std::vector<double> weights;
    for ( int i = 0; i < points.size(); i++ )
      weights.push_back(1.0 );

    int degree = 3;
    std::vector<double> nodes;
    for ( size_t i=0; i<degree+1; ++i ) {
      nodes.push_back( 0. );
    }
    double node = 1.;
    for ( size_t i=0; i< points.size()-degree-1; ++i ) {
      nodes.push_back( node );
      node += 1.;
    }
    for ( size_t i=0; i<degree+1; ++i ) {
      nodes.push_back( node );
    }
    bool isClosed = false;

    return std::make_shared<Math::NurbsCurve>( points, weights, nodes, isClosed, degree );
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
