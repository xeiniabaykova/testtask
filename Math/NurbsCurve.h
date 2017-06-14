////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Nurbs - кривая в двумерном пространстве.\~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef NURBSCURVE_H
#define NURBSCURVE_H
#include "Curve.h"
#include "Point.h"
#include "Range.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"


namespace Math {
////////////////////////////////////////////////////////////////////////////////
//
///  Класс геометрического представления Nurbs - кривой.
/**
*/
////////////////////////////////////////////////////////////////////////////////
class NurbsCurve : public Curve
{
private:
  std::vector<Point>  poles;   ///< Опорные точки для кривой.
  std::vector<double> weights; ///< Веса для опорных точек.
  std::vector<double> nodes;   ///< Границы параметра для опорного полинома.
  bool                isClosed;///< Замкнутость кривой.
  size_t              degree;  ///< Степень кривой.

private:
  NurbsCurve(const NurbsCurve &obj) = delete;
  NurbsCurve& operator=(NurbsCurve &obj) = delete;
public:
  NurbsCurve() = default;
  virtual ~NurbsCurve() = default;
  /**  \brief Создать nurbs - кривую по опорным точкам, весам, опорным точкам и замкнутости.
  \param[in] points - Опорные точки .\~
  \param[in] referencePoints - Опорные точки .\~
  \param[in] weights - Веса .\~
  \param[in] isClosed - Замкнутость .\~
  */
  //---
  NurbsCurve( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
    const std::vector<double>& nnodes, bool iisClosed, size_t ddegree );

  virtual Point       GetPoint               ( double t ) const;                                               ///< Вернуть точку на кривой по параметру t.
  virtual Range       GetRange               () const;                                                         ///< Вернуть границы параметра для Nurbs : [nodes[0], nodes[size]].
  virtual Vector      GetDerivative          ( double t ) const;                                               ///< Вернуть производную на nurbs по параметру t.
  virtual Vector      Get2Derivative         ( double t ) const;                                               ///< Вернуть вторую производную на nurbs по параметру t.
  virtual void        Translate              ( double xShift, double yShift );                                 ///< Сдвиг по оси x на xShift, по оси y на yShift.
  virtual void        Rotate                 ( double alpha );                                                 ///< Повернуть относительно начала координат на угол alpha.
  virtual std::vector<Point>  GetReferensedPoints    () const;                                                         ///< Вернуть опорные точки, использованные для построения nurbs - кривой
  virtual void        Scale                  ( double XScaling, double YScaling );    ///< Масштабировать на xScaling по оси x, на yScaling по оси у.
  std::vector<Point>  GetPoles               () const;                                                         ///< Вернуть опорные точки.
  std::vector<double> GetWeights             () const;                                                         ///< Вернуть веса опорных точек.
  std::vector<double> GetNodes               () const;                                                         ///< Вернуть границы параметра t для базисных полиномов.
  bool                IsClosed               () const;                                                         ///< Вернуть замкнутость Nurbs - кривой.
  size_t              Degree                 () const;                                                         ///< Вернуть степень Nurbs - кривой.
  std::string         GetName                () const;                                                         ///< Вернуть имя, используемое для записи примитива в файл.
  bool                IsValid                () const;                                                         ///< Проверка на правильность кривой.

private:
  std::vector<double> BasicFunctions          ( int i, double x) const;                                         ///< Подсчитать значения базисных функций на отрезке x - degree.
  Vector              CountingDer            ( double t, int der ) const;                                      ///<  Внутренняя функция для посдчета производной порядка der.
  std::vector<Point>  PointDers              ( double t , int der,  const std::vector<std::vector<double>>& ders ) const;                                     ///<  Производные от базисных функций, помноженные на соответсвующие точки кривой.
  std::vector<double> WeightDers             ( double t, int der,  const std::vector<std::vector<double>>& ders ) const;                                      ///<  Производные от базисных функций, помноженные на соответсвующие веса кривой.
  double              CountWeight            ( int k , double x) const;                                        ///< Подсчитать значения базисных функций, умноженных на вес на отрезке x - degree.
  double              CountWeightD           ( double x, int span )  const;                                    ///< Подсчитать значения производных базисных функций, умноженных на вес на отрезке x - degree.
  double              CountWeightD2          ( double x, int span )  const;                                    ///< Подсчитать значения вторых производных базисных функций, умноженных на вес на отрезке x - degree
  size_t              FindSpan               ( double x ) const;                                               ///< Найти интервал ненулевых базисных функций для парметра t.
//ФУНКЦИЯ НУЖНА ДЛЯ ТЕСТОВ И ОТЛАДКИ
  //  double              ComputeBasicFunction   ( double x, int i ) const;                       ///< Подсчитать базисную функцию для порядка i и значения парметра x.
  void                ComputeBasicFunctionD  ( const double x, const int i, const size_t derivativeOrder, std::vector<std::vector<double>>& ders) const;
};
}

#endif // NURBSCURVE_H
