////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  Хранение отображаемой кривой: полилиния, опорные точки и series\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef DISPLAYEDCURVE_H
#define DISPLAYEDCURVE_H

#include <Math/GeometricPrimitive.h>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <Math/GeomPolyline.h>
#include <QtCharts/QValueAxis>
#include <memory>

//------------------------------------------------------------------------------
/** \brief \ru Хранение информации о отображаемой кривой :
 * цвет, селектирвумость, опорные точки и средство для отображения графика в Qchart \~
*/
// ---

QT_CHARTS_USE_NAMESPACE
struct DisplayedCurve
{
private:
  QColor                              currentColor;     ///< цвет неслектированной кривой
  bool                                selected;         ///< свойство селектированности кривой
  std::shared_ptr<QLineSeries>        currentseries;    ///< series с точками
  std::shared_ptr<QScatterSeries>     seriesRef;        ///< series с опорными точками
  std::shared_ptr<C2Curve>            curve;
  QValueAxis*                         axisX;
  QValueAxis*                         axisY;
public:
  DisplayedCurve() = default;
  ~DisplayedCurve();
  DisplayedCurve(std::shared_ptr<C2Curve> curve,  QValueAxis* axisX,  QValueAxis* axisY ):
    currentColor     ( 51, 0, 51        ),
    selected         ( false            ),
    curve( curve ),
    axisX( axisX ),
    axisY( axisY )
  {
  }
private:
  DisplayedCurve( const DisplayedCurve &obj ) = delete;
  DisplayedCurve& operator=( DisplayedCurve &obj ) = delete;
 public:
  void addCurveToChart         ( QChart * chart, double accuracy );
  bool GetSelectionStatus      ();                                              ///< получить информацию о селектированности кривой
  void ModifySelectionStatus   ( Point cursor, double precision, QColor selectedColor );                                ///< изменить информацию о селектированности кривой в зависимости от полученной точки
  void SetColor                ( QColor color );                                ///< установить цвет кривой
  void SetSeries               ( QLineSeries *  current, QScatterSeries *ref ); ///< установить series
  void SetColorUnselectedCurve ( QColor color );
};

#endif // DISPLAYEDCURVE_H
