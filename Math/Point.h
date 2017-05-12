////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru двумерное представление точки\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef POINT_H
#define POINT_H

//------------------------------------------------------------------------------
/** \brief \ru Класс для двумерного представления точки \~
*/
// ---
class Point
{
private:
  double x; ///< координата x
  double y; ///< координата y

public:
  Point() = default;
  /**  \brief \ru создать двумерную точку по координатам x, y
    \param[in] x  - координата .\~
    \param[in] y - координата .\~
  */
  //---
  Point( double x, double y );
  Point( const Point& point );
  virtual ~Point() = default;
public:
  double  GetX() const; ///< получить координату x
  double  GetY() const; ///< получить координату y

  Point operator * ( double param ) const ///< умножить точку на скаляр
  {
    return Point( x * param, y * param );
  }
  Point operator - ( Point point ) const ///< вычесть из одной точки другую точку
  {
    return Point( x - point.GetX(), y - point.GetY() );
  }
  Point operator + ( Point point ) const ///< прибавить к одной точке другую точку
  {
    return Point( x + point.GetX(), y + point.GetY() );
  }
};

#endif // POINT_H
