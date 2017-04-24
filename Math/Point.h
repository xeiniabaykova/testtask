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
};

#endif // POINT_H
