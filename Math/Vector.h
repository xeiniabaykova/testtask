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
  double x1; ///< координата x
  double x2; ///< координата y

public:
  Vector() = default;
  Vector( double x1, double x2 );
  Vector( const Vector& point );
  virtual ~Vector() = default;
public:
  double  GetX1() const;
  double  GetX2() const;
};

#endif // VECTOR_H
