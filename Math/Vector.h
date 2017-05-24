#ifndef VECTOR_H
#define VECTOR_H

////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru двумерный вектор \~

*/
////////////////////////////////////////////////////////////////////////////////
class Vector
{
private:
  double x; ///< координата x
  double y; ///< координата y

public:
  Vector() = default;
  Vector( double x, double y );
  Vector( const Vector& point );
  virtual ~Vector() = default;
public:
  double  GetX() const;
  double  GetY() const;
};

#endif // VECTOR_H
