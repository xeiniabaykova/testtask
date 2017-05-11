#ifndef VECTOR_H
#define VECTOR_H

////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru двумерный вектор \~

*/
////////////////////////////////////////////////////////////////////////////////

#include "Point.h"

//------------------------------------------------------------------------------
/** \brief \ru Класс для двумерного представления точки \~
*/
// ---
class Vector
{
private:
  double x1; ///< координата x
  double x2; ///< координата y

public:
  Vector() = default;
  Vector( double x1, double x2 );
  Vector( const Point& point );
  ~Vector() = default;
public:
  double  GetX1() const;
  double  GetX2() const;
};

#endif // VECTOR_H
