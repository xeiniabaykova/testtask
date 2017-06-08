////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Геометрическое представление эллипса.\~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Curve.h"
#include "Point.h"
#include "Range.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"
#include "Vector.h"


///////////////////////////////////////////////////////////////////////////////
//
/// Класс геометрического представления эллипса.
/**
  Представлет функции для хранения и проведения опраций над эллипсом.
*/
///////////////////////////////////////////////////////////////////////////////
namespace Math {
class Ellipse: public Curve
{
private:
  Point center;  ///< центр
  double r1;     ///< первый радиус
  double r2;     ///< второй радиус
  double alpha;  ///< угол между главной осью и осью х
public:
  /**  \brief  Создать эллипс по массиву точек.
    \param[in] points[0] - Центр эллипса .\~
    \param[in] points[1] - Главная ось эллипса .\~
    \param[in] points[1] - Точка, лежащая на границе эллипса .\~
  */
  //---
  Ellipse ( const std::vector<Point>& points );
  /**  \brief Создать эллипс по двум радиусам и центральной точке.
    \param[in] center - ентр эллипса .\~
    \param[in] r1 - главный радиус .\~
    \param[in] r2 - побочный радиус .\~
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
  virtual Vector             GetDerivativePoint  ( double t ) const;                   ///< Вернуть производную на эллипсе по параметру t.
  virtual Vector             Get2DerivativePoint ( double t ) const;                   ///< Вернуть вторую производную на эллипсе по параметру t.
  virtual void               Translate           ( double xShift, double yShift );     ///< Сдвиг по оси x на xShift, по оси y на yShift.
  virtual void               Rotate              ( double alpha );                     ///< Повернуть относительно начала координат на угол alpha.
  virtual void               Scale               ( double xScaling, double yScaling ); ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  bool                       IsValid             () const;                             ///< Проверить корректность эллипса: считаем, что если оба радиуса не равны нулю, то эллипс корректен.
  double                     GetMajorRadius      () const;                             ///< Вернуть гравный радиус.
  double                     GetMinorRadius      () const;                             ///< Вернуть побочный радиус.
  double                     GetAlpha            () const;                             ///< Вернуть угол наклона относительно оси ох.
  Point                      GetCenter           () const;                             ///< Вернуть центр эллипса.
  std::string                GetName             () const;                             ///< Вернуть имя, используемое при записи эллипса в файл.
  virtual std::vector<Point> GetReferensedPoints () const;                             ///< Вернуть опорные точки, использованные для построения эллипса.
  double                     FixedParameter      ( double t) const;                    ///< Возращает параметр, преобразованный к области определения параметра[0, 2pi], используя свойство переодичности кривой.
};
}
#endif // ELLIPSE_H
