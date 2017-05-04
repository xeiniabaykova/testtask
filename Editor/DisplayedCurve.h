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
#include <memory>

//------------------------------------------------------------------------------
/** \brief \ru Хранение информации о отображаемой кривой :
 * цвет, селектирвумость, опорные точки и средство для отображения графика в Qchart \~
*/
// ---

QT_CHARTS_USE_NAMESPACE
struct DisplayedCurve
{
public:
  DisplayedCurve() = default;
  ~DisplayedCurve();
  /**  \brief \ru Записать всю информацию о кривой:
    \param[in] referencedPoints - \ru опорные точки кривой
     \param[in] currentColor - \ru цвет неселектированной кривой
    \param[in] selectedColor - \ru цвет селектированной кривой
     \param[in] precision - \ru точность построения кривой
     \param[in] polyline - \ru точки, составляющие полилинию
    .\~
  */
  DisplayedCurve( QChart *chart, std::vector<Point> referencedPoints, std::vector<Point> polylinePoints ):
    referencedPoints ( referencedPoints ),
    currentColor     ( 51, 0, 51        ),
    selectedColor    ( 12, 0, 255       ),
    chart            ( chart            ),
    precision        ( 0.1              ),
    polyline         ( polylinePoints   )
  {
  }
private:
  DisplayedCurve( const DisplayedCurve &obj ) = delete;
  DisplayedCurve& operator=( DisplayedCurve &obj ) = delete;

private:
  QColor                              currentColor;    ///< цвет неслектированной кривой
  QColor                              selectedColor;   ///< цвет селектированной кривой
  std::vector<Point>                  referencedPoints; ///< опорные точки
  GeomPolyline                        polyline;         ///< полилиния для данной кривой
  bool                                selected;         ///< свойство селектированности кривой
  double                              precision;        ///< точность построения полилнии
  QLineSeries                       * currentseries;    ///< series с точками
  QScatterSeries                    * seriesRef;        ///< series с опорными точками
  QChart                            * chart;

private:
  double DistanceToPoint( Point point );

 public:
  std::vector<Point>&                  GetReferensedPoints   (                                             );        ///< получить опорные точки
  const  GeomPolyline&                 GetPolyline           (                                             ) const; ///< получить опорные точки
  bool                                 GetSelectionStatus    (                                             );       ///< получить информацию о селектированности кривой
  void                                 ModifySelectionStatus ( Point cursor                                );       ///< изменить информацию о селектированности кривой в зависимости от полученной точки
  void                                 SetColor              ( QColor color                                );       ///< установить цвет кривой
  void                                 SetSeries             ( QLineSeries *  current, QScatterSeries *ref );       ///< установить series

};

#endif // DISPLAYEDCURVE_H
