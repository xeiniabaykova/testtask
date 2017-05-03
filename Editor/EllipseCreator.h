////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель эллипса в двумерном прострастве по точкам\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef ELLIPSECREATOR_H
#define ELLIPSECREATOR_H

#include "GeometricPrimitiveCreator.h"

//------------------------------------------------------------------------------
/** \brief \ru создание геометрического примитива эллипса по трем точкам:
  Point1 - центральная точка эллипса, Point2 - граница главной полуоси, Point3 - точка на границе эллипса \~
*/
// ---
class EllipseCreator: public GeometricPrimitiveCreator {
public:
  EllipseCreator() = default;
  ~EllipseCreator() = default;

private:
  EllipseCreator( const EllipseCreator &obj ) = delete;
  EllipseCreator& EllipseCreator::operator=( EllipseCreator &obj ) = delete;

public:
  /**  \brief \ru Получить эллипс по массиву из точек.
    \param[in] points - \ru Вектор из точек: points[0] - точка в центре эллипса,points[1] - точка, на границе главной полуоси
      points[2] - точка на границе эллипса ( точки не должны лежать на одной прямой).\~
    \return \ru Указатель на базовый класс геометрии.\~
  */
  //---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& points ) const;
};

#endif // ELLIPSECREATOR_H
