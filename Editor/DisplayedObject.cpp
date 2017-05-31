#include "DisplayedObject.h"
#include "CommonConstantsEditor.h"

//-----------------------------------------------------------------------------
/**
  \ru вернуть текущее состояние селекции
*/
//-----------------------------------------------------------------------------
bool DisplayedObject::GetSelectionStatus()
{
  return selected;
}


//-----------------------------------------------------------------------------
/**
  \ru изменить состояние селекции в зависимости от полученной точки
*/
//-----------------------------------------------------------------------------
void DisplayedObject::ModifySelectionStatus( Math::Point cursor, double precision, QColor selectedColor )
{
 //if ( curve->DistanceToPoint( cursor ) < precision )
    selected = !selected;
 if (selected)
   SetColor( selectedColor );
 else
   SetColor( currentColor );
}


//-----------------------------------------------------------------------------
/**
  \ru установить новый цвет для неселектированной кривой
*/
//-----------------------------------------------------------------------------
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
  \ru добавить series к кривой
*/
//-----------------------------------------------------------------------------
void DisplayedObject::SetSeries( QLineSeries *  current, QScatterSeries *ref )
{
  currentseries = current;
  seriesRef = ref;
}


//-----------------------------------------------------------------------------
/**
  \ru убрать series из графика
*/
//-----------------------------------------------------------------------------
 DisplayedObject::~DisplayedObject()
{
  currentseries->chart()->removeSeries( currentseries );
  seriesRef->chart()->removeSeries( seriesRef );
}

void DisplayedObject::addCurveToChart( QChart * chart)
{
  currentseries = new QLineSeries;
  currentseries->setColor( currentColor );

  seriesRef = new QScatterSeries;
  seriesRef->setColor( currentColor );

  std::vector<Math::Point> polyPoints;
  curve->GetAsPolyLine( polyPoints,Math::CommonConstantsMath::PRECISION_POLYLINE );

  for ( int i = 0; i < polyPoints.size(); i++ )
    *currentseries <<QPointF( polyPoints[i].GetX(), polyPoints[i].GetY() );

  //for ( int i = 0; i < curve->GetReferensedPoints().size(); i++ )
   //   *seriesRef << QPointF( curve->GetReferensedPoints()[i].GetX(), curve->GetReferensedPoints()[i].GetY() );

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries );
  chart->addSeries( seriesRef );


  currentseries->attachAxis( axisX );
  currentseries->attachAxis( axisY );
  seriesRef->attachAxis( axisX );
  seriesRef->attachAxis( axisY );
}
