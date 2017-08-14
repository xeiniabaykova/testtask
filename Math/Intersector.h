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
#include <map>


namespace Math {


using Intersections = std::map<std::pair<const Curve*, const Curve*>, std::vector<std::pair<double, double>>>;
  
  /**  \brief Найти пересечения кривых
  \param[in] curves - вектор указателей на кривые .\~
  \ param[in] accuracyPolyliline - точность апроксимации кривой полилинией .\~
  \param[out] Множество данных о пересечениях.\~
  */
  //---
void Intersect( const std::vector<Curve*>& curves, Intersections& intersectionPoints, const double& accuracyPolyliline = CommonConstantsMath::PRECISION_POLYLINE );
}

#endif // INTERSECTOR_H
