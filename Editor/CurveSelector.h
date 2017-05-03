////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  селектор полилиний по признаку близости к точке\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CURVESELECTOR_H
#define CURVESELECTOR_H

#include <Math/Point.h>
#include <vector>

//------------------------------------------------------------------------------
/** \brief \ru Селектирование кривых, отображенных на экране:
  расчитывается расстояние от входной точки до каждой из полилиний \~
*/
// ---
class CurveSelector
{
private:
  const std::vector<std::vector<Point>>& points; ///< полилинии экранных кривых
  double accuracy;                               ///< точность селектирования

public:
  CurveSelector() = default;
  CurveSelector( const std::vector<std::vector<Point>>& points );
  ~CurveSelector() = default;

private:
  CurveSelector( const CurveSelector &obj ) = delete;
  CurveSelector& CurveSelector::operator=( CurveSelector &obj ) = delete;

public:
  /**  \brief \ru  получить номер кривой, расстояние от которой до point меньше допустимого.
    \param[in] point - \ru точка, до которой считается расстояние.
      \return \ru номер кривой, расстояние до которой меньше допустимого. Если такой кривой нет, возвращается -1\~
  */
  //---
  int GetCurve( Point point ) const;

};

#endif // CURVESELECTOR_H
