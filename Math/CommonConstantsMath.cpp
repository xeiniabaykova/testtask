#include "CommonConstantsMath.h"

namespace Math {

const double CommonConstantsMath::PI = 3.14159265358979323846;  ///<Число Pi.
const double CommonConstantsMath::PRECISION_POLYLINE = 0.0005;  ///<Точность созания полилнии.
const double CommonConstantsMath::NULL_TOL = 1e-7;     ///<  Нулевая точность. Считаем, что данной погрешностью можно пренебречь, и все, что меньше этого числа, равно нулю.
const double CommonConstantsMath::SCALING = 1e-10;     ///< Точность захода за границы отрезков для алгоритма пересечения. (необходимо, чтобы учитывать случай начало отрезка - конец  отрезка).
const std::size_t CommonConstantsMath::NUMBER_NEWTON_METHOD = 20; ///< Максимальное оличество итераций метода Ньютона.
const double CommonConstantsMath::ACCURANCY_METHOD_NEWTON = 1e-8; ///< Точность метода Ньютона.

}
