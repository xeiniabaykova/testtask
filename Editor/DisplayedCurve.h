////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  хранение геометрического примитива и опорных точек\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef DISPLAYEDCURVE_H
#define DISPLAYEDCURVE_H

#include <Math/GeometricPrimitive.h>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <memory>

//------------------------------------------------------------------------------
/** \brief \ru Хранение геометрического примитива и опорных точек:
  опорные точки - точки, на основе которых строится геометрический примитив \~
*/
// ---

QT_CHARTS_USE_NAMESPACE
struct DisplayedCurve
{
public:
  DisplayedCurve() = default;
  DisplayedCurve( QChart *chart ):
    currentColor     ( 51, 0, 51         ),
    selectedColor    ( 12, 0, 255        ),
    chart            ( chart             )
  {
  }

  ~DisplayedCurve();
  DisplayedCurve( std::vector<Point> referencedPoints, std::shared_ptr<GeometricPrimitive> primitive ):
    referencedPoints ( referencedPoints ),
    primitive        ( primitive        ),
    currentColor     ( 51, 0, 51         ),
    selectedColor    ( 12, 0, 255        )
  {
    presision = 0.01;
  }
private:
  DisplayedCurve( const DisplayedCurve &obj ) = delete;
  DisplayedCurve& operator=( DisplayedCurve &obj ) = delete;

private:
  QColor                              currentColor;
  QColor                              selectedColor;
  std::vector<Point>                  referencedPoints; ///< опорные точки
  std::vector<Point>                  polyline;
  std::shared_ptr<GeometricPrimitive> primitive;        ///< геометрический примитив
  bool                                selected;       ///< свойство селектированности кривой
  double                              presision;
  QLineSeries *                       currentseries;
  QScatterSeries                      *seriesRef;
  QChart                              *chart;
private:
  double DistanceToPoint( Point point );
 public:
  std::vector<Point>&                  GetReferensedPoints(); ///< получить опорные точки
  std::shared_ptr<GeometricPrimitive>& GetPrimitive();        ///< получить геометрический примитив
  std::vector<Point>&                  GetPolyline(); ///< получить опорные точки
  bool GetSelectionStatus();
  void ModifySelectionStatus( Point cursor );
  void SetColor (QColor color);
  void SetSeries( QLineSeries *  current, QScatterSeries *ref);

};

#endif // DISPLAYEDCURVE_H
