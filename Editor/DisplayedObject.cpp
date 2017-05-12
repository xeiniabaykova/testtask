#include "DisplayedObject.h"

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
void DisplayedObject::ModifySelectionStatus( Point cursor, double precision, QColor selectedColor )
{
 if ( curve->DistancePointToCurve( cursor ) < precision )
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
  currentseries = std::make_shared<QLineSeries>( current);
  seriesRef = std::make_shared<QScatterSeries>(ref);
}


//-----------------------------------------------------------------------------
/**
  \ru убрать series из графика
*/
//-----------------------------------------------------------------------------
 DisplayedObject::~DisplayedObject()
{
  currentseries->chart()->removeSeries( currentseries.get() );
  seriesRef->chart()->removeSeries( seriesRef.get() );
}

void DisplayedObject::addCurveToChart( QChart * chart, double accuracy )
{
  currentseries = std::make_shared<QLineSeries>();
  currentseries->setColor( currentColor );

  seriesRef = std::make_shared<QScatterSeries>();
  seriesRef->setColor( currentColor );

  std::vector<Point> polyPoints;
  curve->GetAsPolyLine( polyPoints,accuracy );

  for ( int i = 0; i < polyPoints.size(); i++ )
    *currentseries <<QPointF( polyPoints[i].GetX(), polyPoints[i].GetY() );

  for ( int i = 0; i < curve->GetReferensedPoints().size(); i++ )
      *seriesRef << QPointF( curve->GetReferensedPoints()[i].GetX(), curve->GetReferensedPoints()[i].GetY() );

  seriesRef->setMarkerShape( QScatterSeries::MarkerShapeCircle );
  seriesRef->setMarkerSize( 15.0 );
  chart->addSeries( currentseries.get() );
  chart->addSeries( seriesRef.get() );


  currentseries->attachAxis( axisX.get() );
  currentseries->attachAxis( axisY.get() );
  seriesRef->attachAxis( axisX.get() );
  seriesRef->attachAxis( axisY.get() );
}
