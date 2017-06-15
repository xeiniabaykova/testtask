#ifndef RANGE_H
#define RANGE_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Интрервал с включенными границами.\~

*/
////////////////////////////////////////////////////////////////////////////////


namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
/// Класс представления интервала.
/**
 Интевал преставляется двумя границами
  : началом и концом. Считаем, что начало всегда меньше конца.
*/
///////////////////////////////////////////////////////////////////////////////
class Range {
private:
  double start; ///< начало
  double end; ///< конец
public:
  Range() = default;
  /**  \brief создать интервал по началу и концу интервала
    \param[in] start - начало интервала .\~
    \param[in] end - конец интервала .\~
  */
  //---
  Range( double start, double end );
  Range(const Range& range);
  virtual ~Range() = default;

public:
  double GetStart  () const; ///< Получить конец интервала.
  double GetEnd    () const; ///< Получить конец интервала.
  bool   IsValid   () const; ///< Получить конец интервала.
  double Lenght    () const; ///< Размер интервала.
};
}
#endif // RANGE_H

