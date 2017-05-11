////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  точка в двумерном пространстве (возможно, больше не нужно)\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRICPOINT_H
#define GEOMETRICPOINT_H
#include <vector>
#include "Point.h"
#include "GeometricPrimitive.h"


//------------------------------------------------------------------------------
/** \brief \ru Класс- заплатка для точки.
 * Используется для упрощения создания и отображения геометрических объектов  \~
**/
// ---
class GeometricPoint: public GeometricPrimitive
{
private:
  Point point; ///< точка
public:
  //-------------------------------------------------------------------------------
  // \ru Создание точки по точке
  // ---
  GeometricPoint ( Point point );
  virtual void   Translation( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Rotation( const std::vector<std::vector<double>>& transformationMatrix );
  virtual void   Dilatation( const std::vector<std::vector<double>>& transformationMatrix );
};


#endif // GEOMETRICPOINT_H
