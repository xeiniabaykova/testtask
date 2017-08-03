#ifndef INTERSECTOR_H
#define INTERSECTOR_H
////////////////////////////////////////////////////////////////////////////////
/**
  Файл, содержащий функции для пересечения геометрических примитивов.\~

*/
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "Point.h"
#include "Curve.h"
#include "Line.h"

namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
///  Тип для представления данных о пересечении. 
/**
   Тип для представления данных о пересечении двух параметрических кривых curve1 и curve2. 
   Значения параметров curveparameter1 и paramcurve2 в этих кривых соответствуют точке пересечения.
*/
///////////////////////////////////////////////////////////////////////////////
struct CurveIntersectionData
{
private:
  const Curve& curve1;      // Указатель на первую кривую. 
  const Curve& curve2;      // Указатель на вторую кривую. 
  double       paramCurve1; // Параметр первой кривой, соответствующий точке пересечения.
  double       paramCurve2; // Параметр второй кривой, соответствующий точке пересечения.
public:
  CurveIntersectionData() = default;
  virtual ~CurveIntersectionData() = default;
  CurveIntersectionData( const CurveIntersectionData &obj ) = default;
  CurveIntersectionData& operator=( CurveIntersectionData &obj ) = delete;

  /**  \brief Создать данные о пересечении двух параметрических кривых.
  \param[in] theCurve1 - Первая кривая .\~
  \param[in] theCurve1 - Вторая кривая .\~
  \param[int] theParamPoint - Набор парметров, при которых кривые пересекаются.\~
  */
  //---
  CurveIntersectionData( const Curve& theCurve1, const Curve& theCurve2, std::pair<double, double> theParamPoint ):
    curve1      ( theCurve1 ),
    curve2      ( theCurve2 ),
    paramCurve1 ( theParamPoint.first ),
    paramCurve2 ( theParamPoint.second )
  {}
  /// Вернуть первую кривую.
  const Curve&              GetCurve1() const;
  /// Вернуть вторую кривую.
  const Curve&             GetCurve2() const;
  /// Вернуть набор параметров, при которых кривые пересекаются.
  std::pair<double, double> GetParams();
};
  /**  \brief Найти пересечения кривых
  \param[in] curves - вектор указателей на кривые .\~
  \param[out] Вектор данных о пересечениях.\~
  */
  //---
std::vector<CurveIntersectionData> Intersect( const std::vector<Curve*>& curves );
}

#endif // INTERSECTOR_H
