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
  const Curve* curve1;      // Указатель на первую кривую. 
  const Curve* curve2;      // Указатель на вторую кривую. 
  double       paramCurve1; // Параметр первой кривой, соответствующий точке пересечения.
  double       paramCurve2; // Параметр второй кривой, соответствующий точке пересечения.
  CurveIntersectionData( const Curve* theCurve1, const Curve* theCurve2, std::pair<double, double> theParamPoint ):
    curve1      ( theCurve1 ),
    curve2      ( theCurve2 ),
    paramCurve1 ( theParamPoint.first ),
    paramCurve2 ( theParamPoint.second )
  {}

  std::pair<double, double> GetParamPoint()
  {
    return std::make_pair( paramCurve1, paramCurve2 );
  }

};
/**  \brief Найти пересечения кривых
\param[in] curves - вектор указателей на кривые .\~
\param[out] Вектор данных о пересечениях.\~
*/
//---
std::vector<CurveIntersectionData> Intersect( const std::vector<Curve*>& curves );
}

#endif // INTERSECTOR_H
