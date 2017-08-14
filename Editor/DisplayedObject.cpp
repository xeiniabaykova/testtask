#include "DisplayedObject.h"
#include "CommonConstantsEditor.h"
#include "Math/GeomPolyline.h"

namespace Editor {

DisplayedObject::DisplayedObject( std::shared_ptr<Math::Curve> curve ):
  currentColor ( 51, 0, 51 ),
  selected     ( false ),
  curve        ( curve )
{
}


//-----------------------------------------------------------------------------
//  Вернуть текущее состояние селекции.
// ---
bool DisplayedObject::GetSelectionStatus()
{
  return selected;
}


//-----------------------------------------------------------------------------
//  Изменить состояние селекции в зависимости от полученной точки.
// ---
void DisplayedObject::ModifySelectionStatus( Math::Point cursor, double precision, QColor selectedColor )
{
  Math::GeomPolyline polyline;
  curve->GetAsPolyLine( polyline );
  if ( polyline.DistanceToPoint(cursor) < precision )
    selected = !selected;
  if ( selected )
   SetColor( selectedColor );
  else
   SetColor( currentColor );
}


//-----------------------------------------------------------------------------
//  Установить новый цвет для селектированной кривой.
// ---
void DisplayedObject::SetColor( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
}


//-----------------------------------------------------------------------------
//  Установить новый цвет для неселектированной кривой.
// ---
void DisplayedObject::SetColorUnselectedCurve( QColor color )
{
  currentseries->setColor( color );
  seriesRef->setColor( color );
  currentColor = color;
}


//-----------------------------------------------------------------------------
//  Убрать series из графика.
// ---
 DisplayedObject::~DisplayedObject()
{
  currentseries->chart()->removeSeries( currentseries );
  seriesRef->chart()->removeSeries( seriesRef );
}


//-----------------------------------------------------------------------------
// Добавить кривую к пространству отрисовки.
// ---
void DisplayedObject::AddCurveToChart( QChart * chart)
{
  currentseries = new QLineSeries;
  currentseries->setColor( currentColor );

  seriesRef = new QScatterSeries;
  seriesRef->setColor( currentColor );

  Math::GeomPolyline polyPoints;
  curve->GetAsPolyLine( polyPoints );
  std::vector<Math::Point> refPoints1;
  polyPoints.GetReferensedPoints(refPoints1);
  for ( size_t i = 0; i < refPoints1.size(); i++ )
    *currentseries << QPointF( refPoints1[i].GetX(), refPoints1[i].GetY() );

  std::vector<Math::Point> refPoints2;
  curve->GetReferensedPoints(refPoints2);
  for ( size_t i = 0; i <refPoints2.size(); i++ )
     *seriesRef << QPointF( refPoints2[i].GetX(),refPoints2[i].GetY() );

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );


  currentseries->attachAxis( chart->axisX() );
  currentseries->attachAxis( chart->axisY() );
  seriesRef->attachAxis( chart->axisX() );
  seriesRef->attachAxis( chart->axisY() );
}



//-----------------------------------------------------------------------------
//  Вернуть геометрический примитив.
// ---
std::shared_ptr<Math::Curve> DisplayedObject::GetPrimitive()
{
  return curve;
}


//-----------------------------------------------------------------------------
//  Удалить опорные точки.
// ---
void DisplayedObject::DelRefSeries()
{
  seriesRef->clear();
}
}
