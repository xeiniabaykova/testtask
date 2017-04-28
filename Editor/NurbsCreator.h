#ifndef NURBSCREATOR_H
#define NURBSCREATOR_H

#include "GeometricPrimitiveCreator.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс для создания геометрического представления NURBS. \~
*/
// ---
class NurbsCreator: public GeometricPrimitiveCreator {
public:
  //-------------------------------------------------------------------------------
  // \ru создает NURBS
  // ---
  virtual std::shared_ptr<GeometricPrimitive> Create( const std::vector<Point>& referencePoints,
                                                      const std::vector<double>& weights,
                                                      const std::vector<int>& nodes,
                                                      bool isClosed );
};

#endif // NURBSCREATOR_H
