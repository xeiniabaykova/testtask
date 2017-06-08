#include "CommonConstantsMath.h"

namespace Math {

double CommonConstantsMath::PI = 3.14159265358979323846;  ///<Число Pi.
double CommonConstantsMath::PRECISION_POLYLINE = 0.001;  ///< Нулевая точность. Считаем, что данной погрешностью можно пренебречь, и все, что меньше этого числа, равно нулю.
double CommonConstantsMath::NULL_TOL = 1e-7;             ///< Точность создания полилинии.

}
