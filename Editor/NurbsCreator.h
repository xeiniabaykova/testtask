////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  создатель Nurbs - кривой по набору параметров\~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef NURBSCREATOR_H
#define NURBSCREATOR_H

#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления NURBS. \~
*/
// ---
class NurbsCreator: public GeometricPrimitiveCreator {
public:
  NurbsCreator() = default;
  ~NurbsCreator() = default;
private:
  NurbsCreator( const NurbsCreator &obj ) = delete;
  NurbsCreator& NurbsCreator::operator=( NurbsCreator &obj ) = delete;
public:
  /**  \brief \ru Получить Nurbs - кривую по входным парметрам.
    \param[in] referencePoints - \ru опорные точки .\~
    \param[in] weights - \ru веса .\~
     \param[in] isClosed - \ru замкнутость .\~
    \return \ru Указатель на геометрическое представление Nurbs - кривой.\~
  */
  //---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& referencePoints, const std::vector<double>& weights,
                                                      const std::vector<int>& nodes, bool isClosed );

};

#endif // NURBSCREATOR_H
