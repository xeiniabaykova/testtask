#include "DisplayedObject.h"
#include "CommonConstantsEditor.h"
#include "Math/GeomPolyline.h"

namespace Editor {

DisplayedObject::DisplayedObject( std::shared_ptr<Math::Curve> curve,  QValueAxis* axisX,  QValueAxis* axisY ):
  currentColor ( 51, 0, 51 ),
  selected     ( false ),
  curve        ( curve ),
  axisX        ( axisX ),
  axisY        ( axisY )
{
}
//-----------------------------------------------------------------------------
/**
  Вернуть текущее состояние селекции.
*/
//---
bool DisplayedObject::GetSelectionStatus()
{
  return selected;
}


//-----------------------------------------------------------------------------
/**
  Изменить состояние селекции в зависимости от полученной точки.
*/
//---
void DisplayedObject::ModifySelectionStatus( Math::Point cursor, double precision, QColor selectedColor )
{
  Math::GeomPolyline polyline;
  curve->GetAsPolyLine( polyline );
 if ( polyline.DistanceToPoint(cursor) < precision )
    selected = !selected;
 if (selected)
   SetColor( selectedColor );
 else
   SetColor( currentColor );
}


//-----------------------------------------------------------------------------
/**
  Установить новый цвет для неселектированной кривой.
*/
//---
void DisplayedObject::SetColor( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
}

void DisplayedObject::SetColorUnselectedCurve( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
  currentColor = color;
}


//-----------------------------------------------------------------------------
/**
  Убрать series из графика.
*/
//---
 DisplayedObject::~DisplayedObject()
{
  currentseries->chart()->removeSeries( currentseries );
  seriesRef->chart()->removeSeries( seriesRef );
}


 //-----------------------------------------------------------------------------
 /**
   Добавить кривую к пространству отрисовки.
 */
 //---
void DisplayedObject::addCurveToChart( QChart * chart)
{
  currentseries = new QLineSeries;
  currentseries->setColor( currentColor );

  seriesRef = new QScatterSeries;
  seriesRef->setColor( currentColor );

  Math::GeomPolyline polyPoints;
  curve->GetAsPolyLine( polyPoints );

  for ( int i = 0; i < polyPoints.GetReferensedPoints().size(); i++ )
    *currentseries << QPointF( polyPoints.GetReferensedPoints()[i].GetX(), polyPoints.GetReferensedPoints()[i].GetY() );

  for ( int i = 0; i < curve->GetReferensedPoints().size(); i++ )
     *seriesRef << QPointF( curve->GetReferensedPoints()[i].GetX(), curve->GetReferensedPoints()[i].GetY() );

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );


  currentseries->attachAxis( axisX );
  currentseries->attachAxis( axisY );
  seriesRef->attachAxis( axisX );
  seriesRef->attachAxis( axisY );
}
}
