#ifndef ELLIPSE_H
#define ELLIPSE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Геометрическое представление эллипса.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include "Curve.h"
#include "Point.h"
#include "Range.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"
#include "Vector.h"


namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления эллипса.
/**
  Представлет функции для хранения и проведения опраций над эллипсом.
*/
///////////////////////////////////////////////////////////////////////////////
class Ellipse: public Curve
{
private:
  Point center;  ///< Центр.
  double r1;     ///< Главный радиус.
  double r2;     ///< Побочный радиус.

  double alphaMajorAxis;  ///< Угол между главной осью и осью х.
  double alphaMinorAxis; ///< Угол между побочной осью и осью x.
public:
  /**  \brief  Создать эллипс по массиву точек.
    \param[in] Points[0] - Центр эллипса .\~
    \param[in] Points[1] - Главная ось эллипса .\~
    \param[in] Points[1] - Точка, лежащая на границе эллипса .\~
  */
  //---
  Ellipse ( const std::vector<Point>& points );
  /**  \brief Создать эллипс по двум радиусам и центральной точке.
    \param[in] Center - ентр эллипса .\~
    \param[in] R1 - главный радиус .\~
    \param[in] R2 - побочный радиус .\~
  */
  //---
  Ellipse ( Point center, double r1, double r2, double alpha );
  Ellipse () = default;
  virtual ~Ellipse() = default;

private:
  Ellipse( const Ellipse &obj ) = delete;
  Ellipse& operator=( Ellipse &obj ) = delete;

public:
  virtual Point              GetPoint            ( double t ) const;                   ///< Вернуть точку на кривой по параметру t.
  virtual Range              GetRange            () const;                             ///< Вернуть границы параметра для эллипса : [0, 2 * pi].
  virtual Vector             GetDerivative       ( double t ) const;                   ///< Вернуть производную на эллипсе по параметру t.
  virtual Vector             Get2Derivative      ( double t ) const;                   ///< Вернуть вторую производную на эллипсе по параметру t.
  virtual void               Translate           ( double xShift, double yShift );     ///< Сдвинуть по оси x на xShift, по оси y на yShift.
  virtual void               Rotate              ( double alpha );                     ///< Повернуть относительно начала координат на угол alpha.
  virtual void               Scale               ( double xScaling, double yScaling ); ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  virtual std::vector<Point> GetReferensedPoints () const;                             ///< Вернуть опорные точки, использованные для построения эллипса.
  virtual bool               IsClosed            () const;                             ///< Эллипс по умолчанию - замкунтая фигура.
  virtual std::string        GetName             () const;                             ///< Вернуть имя, используемое при записи эллипса в файл.
  bool                       IsValid             () const;                             ///< Проверить корректность эллипса: считаем, что если оба радиуса не равны нулю, то эллипс корректен.
  double                     GetMajorRadius      () const;                             ///< Вернуть гравный радиус.
  double                     GetMinorRadius      () const;                             ///< Вернуть побочный радиус.
  double                     GetAlphaMajorAxis   () const;                             ///< Вернуть угол наклона главной оси относительно оси ох.
  double                     GetAlphaMinorAxis   () const;                             ///< Вернуть угол наклона побочной оси относительно оси ох.
  Point                      GetCenter           () const;                             ///< Вернуть центр эллипса.

};
}
#endif // ELLIPSE_H
