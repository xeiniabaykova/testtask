////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru двумерное представление точки\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef POINT_H
#define POINT_H

//------------------------------------------------------------------------------
/** \brief \ru Класс для фактического представления точки \~
*/
// ---
class Point
{
private:
  double x; // координата x
  double y; // координата y
public:
  //-------------------------------------------------------------------------------
  // \ru создание точки по координатам x, y.
  Point() {}
  Point   ( double x, double y );
  Point   ( const Point& point );
  double  GetX() const; // возвращает координату x
  double  GetY() const; // возвращает координату y
  Point operator * (double param) const
  {
 return Point (x * param, y * param);
  }
  Point operator - ( Point point ) const
  {
    return Point (x - point.GetX(), y - point.GetY());
  }
  Point operator + ( Point point ) const
  {
    return Point (x + point.GetX(), y + point.GetY());
  }
};

#endif // POINT_H
