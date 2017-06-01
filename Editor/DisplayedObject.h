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
 * цвет, селектирвумость, опорные точки и средство для отображения графика в Qchart. \~
*/
// ---

QT_CHARTS_USE_NAMESPACE
namespace Editor {
struct DisplayedObject
{
private:
  QColor                              currentColor;  ///< Цвет неслектированной кривой.
  bool                                selected;      ///< Свойство селектированности кривой.
  QLineSeries*                        currentseries; ///< Series с точками.
  QScatterSeries*                     seriesRef;     ///< Series с опорными точками.
  std::shared_ptr<Math::Curve>        curve;         ///< Геометрическая кривая.
  QValueAxis*                         axisX;         ///< Ось ох.
  QValueAxis*                         axisY;         ///< Ось оy.
public:
  DisplayedObject() = default;
  virtual ~DisplayedObject();
  DisplayedObject( std::shared_ptr<Math::Curve> curve,  QValueAxis* axisX,  QValueAxis* axisY );
private:
  DisplayedObject( const DisplayedObject &obj ) = delete;
  DisplayedObject& operator=( DisplayedObject &obj ) = delete;
 public:
  void addCurveToChart         ( QChart * chart );                                             ///< Добавить кривую к пространству отрисовки.
  bool GetSelectionStatus      ();                                                             ///< Получить информацию о селектированности кривой
  void ModifySelectionStatus   ( Math::Point cursor, double precision, QColor selectedColor ); ///< Изменить информацию о селектированности кривой в зависимости от полученной точки
  void SetColor                ( QColor color );                                               ///< Установить цвет кривой.
  void SetColorUnselectedCurve ( QColor color );                                               ///< Установить цвет неселектированной кривой.
};
}
#endif // DISPLAYEDCURVE_H
