////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Основные математические константы.\~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef COMMONCONSTANTSMATH_H
#define COMMONCONSTANTSMATH_H


namespace Math {
///////////////////////////////////////////////////////////////////////////////
//
/// Структура, содержащая основные математические константы.
/**
*/
///////////////////////////////////////////////////////////////////////////////
struct CommonConstantsMath {
  static const double PI;                  ///< Число Pi.
  static const double NULL_TOL;            ///< Нулевая точность. Считаем, что данной погрешностью можно пренебречь, и все, что меньше этого числа, равно нулю.
  static const double PRECISION_POLYLINE;  ///< Точность создания полилинии.

};
}

#endif // COMMONCONSTANTS_H
