#include "NurbsCurve.h"
#include <cmath>

namespace Math {


namespace {

//-----------------------------------------------------------------------------
//	Подсчитать фаториал числа n.
// ---
static double fact( int N )
{
  if ( N < 0 )
    return 0;
  if ( N == 0 )
    return 1;
  else
    return N * fact( N - 1 );
}


//-----------------------------------------------------------------------------
//  Подсчитать биномиальные коэффициенты от числел n, k.
// ---
static double Bin( int n, int k )
{
  return fact( n ) / (fact( k ) * fact( n - k) );
}



//-----------------------------------------------------------------------------
//  Примитивная проверка для корректности данных длшя nurbs - кривой.
// ---
static bool IsCorrectNurbs( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
  const std::vector<double>& nnodes, bool iisClosed, size_t ddegree )
{
  return !( ppoles.size() == 0 && ppoles.size() != wweights.size()
      && ppoles.size() < static_cast<size_t>(ddegree - 1) );

}
}

////////////////////////////////////////////////////////////////////////////////
//
///
/**
 Класс геометрического представления Nurbs - кривой. \~
*/
////////////////////////////////////////////////////////////////////////////////

  /**  \brief Создать nurbs - кривую по опорным точкам, весам, опорным точкам и замкнутости.
  \param[in] points - опорные точки .\~
  \param[in] referencePoints - Опорные точки .\~
  \param[in] weights - Веса .\~
  \param[in] isClosed - Замкнутость .\~
  */
  //---
NurbsCurve::NurbsCurve( const std::vector<Point>& ppoles, const std::vector<double>& wweights,
  const std::vector<double>& nnodes, bool iisClosed, size_t ddegree ):
  Curve    (),
  poles    (),
  weights  (),
  nodes    (),
  isClosed (),
  degree   ()
{

  if ( (IsCorrectNurbs(ppoles, wweights, nnodes, iisClosed, ddegree)) )
  {
    poles = ppoles;
    weights = wweights;
    isClosed = iisClosed;
    degree = ddegree;
    if ( isClosed )
    {
      for ( size_t i = 0; i < degree ; ++i)
      {
        weights.push_back( weights[i] );
        poles.push_back( poles[i] );
      }
      for ( int i = degree; i > 0; --i )
      {
        nodes.push_back( -i );
      }
      for ( size_t i = 0; i <= ppoles.size() + degree ; ++i ) {
        nodes.push_back( i );
      }
      } else
      {
        for ( size_t i = 0; i < degree + 1; ++i ) {
          nodes.push_back( 0. );
        }
        double node = 1.;
        for ( size_t i = 0; i < static_cast<size_t>( ppoles.size() - degree - 1 ); ++i ) {
          nodes.push_back( node );
          node += 1.;
        }
        for ( size_t i = 0; i < degree + 1; ++i ) {
          nodes.push_back( node );
        }

      }

  }
}


//-----------------------------------------------------------------------------
//  Подсчитать базисную функцию для порядка i и значения парметра x.
// ---
//double NurbsCurve::ComputeBasicFunction( double x, int i) const
//{
//  double result = 0.;
//  std::vector<double> N;
//  N.resize( degree + 1 );

//  // Если считаем базисную функцию нулевого порядка, и x принадлежит параметрическому интервалу, на котором
//  // функция не равна нулю, то возвращаем 1.
//  if ( (i == 0 && x == nodes[0]) ||
//    (i == nodes.size() - degree - 2 && x == nodes[nodes.size() - 1]) )
//  {
//    return 1.0;
//  }

//  // Если считаем базисную функцию нулевого порядка и х не принадлежит параметрическому интервалу, на котором
//  // функция не равна нулю, то возвращаем 1.
//  if ( x < nodes[i] || x >= nodes[i + degree + 1] )
//  {
//    return 0.0;
//  }

//  // Заполняем перевый уровень треугольника базисных функций: на интервале, которому принадлежит x, базисная функция равна единице,
//  // на остальных интервалах базисная функция равна нулю.
//  for ( size_t j = 0; j <= degree; j++ )
//  {
//    if ( x >= nodes[i + j] && x < nodes[i + j + 1] )
//      N[j] = 1.0;
//    else
//      N[j] = 0.0;
//  }
//  // Считаем треугольник базисных функций.
//  for ( size_t k = 1; k <= degree; k++ )
//  {
//    double saved = 0.0;
//    if ( N[0] == 0.0 )
//      saved = 0.0; // в случае нулевого правого элемента сохраненная сумма равна нулю
//    else
//      saved = ( (x - nodes[i]) * N[0]) / (nodes[i + k] - nodes[i] ); // в случае ненулевого правого элемента расчитываем первое слагаемое формулы:
//    // https://wikimedia.org/api/rest_v1/media/math/render/svg/c3c122619a162127d00e225df14573786778abb6
//    for ( size_t j = 0; j < degree - k + 1; j++ )
//    {
//      const double uLeft = nodes[i + j + 1]; // Запоминаем опорные узлы для формулы
//      const double uRight = nodes[i + j + k + 1]; //
//      if ( N[j + 1] == 0.0 ) // если правое левое слагаемое формулы равно нулю
//      {
//        N[j] = saved;
//        saved = 0.0;
//      }
//      else
//      { // если оба слагаемых формулы не равны нулю, высчитываем по ней следующий элемент
//        double temp = N[j + 1] / ( uRight - uLeft );
//        N[j] = saved + ( uRight - x ) * temp;
//        saved = ( x - uLeft ) * temp;
//      }
//    }
//  }
//  return N[0]; // возвращаем результат

//}


//-----------------------------------------------------------------------------
//  Найти интервал ненулевых базисных функций для парметра t.
// ---
size_t NurbsCurve::FindSpan( double x ) const
{
  // ищем промежуток, на котором базисные функции не будут равны нулю.
  size_t n = 0;
  size_t low = degree;
  n = nodes.size() - degree - 1;
  low = degree;

  if ( x == nodes[n] ) return n - 1;

  size_t hight = n + 1;
  size_t mid = ( low + hight ) / 2;
  while ( (x < nodes[mid]) || (x >= nodes[mid + 1]) )
  {
    if ( x < nodes[mid] )
      hight = mid;
    else
      low = mid;
    mid = ( low + hight ) / 2;
  }
  return mid;
}


//-----------------------------------------------------------------------------
//  Подсчитать массив производных  от базисной функции для интервала [i-degree, i] и значения парметра x, где derivativeOrder - порядок производной.
//  Возвращаемый параметр - массив ders - где ders[k][j], где k - порядок производной, j - параметр ненулевого интервала от 0 до degree, для которого вычислена производная.
// ---
void NurbsCurve::ComputeBasicFunctionD( const double x, const int i, const size_t derivativeOrder, std::vector<std::vector<double>>& ders ) const
{

  // формула для подсчета производной от базисной функции порядка i и значения параметра x имеет вид:
  // ders_(i,degree)^(derivativeOrder) = degree/(degree - derivativeOrder)* [(x - nodes_i)/ (nodes_(i+degree) - nodes_i)
  // * ders^(derivativeOrder)_(i, degree - 1) + (nodes_(i + degree + 1) - x)/ (nodes_(i + degree +1) - nodes_(i+1)) * ders^(derivativeOrder)_(i + 1, degree-1)]; (1)

  ders.resize( derivativeOrder + 1 );
  for ( size_t k = 0; k < ders.size(); k++ )
    ders[k].resize( degree + 1 );

  std::vector<std::vector<double>> triangleNodes; // хранение базисных функций
  triangleNodes.resize( degree + 1 );
  for ( size_t k = 0; k < triangleNodes.size(); k++ )
    triangleNodes[k].resize( degree + 1 );
  triangleNodes[0][0] = 1.0;


  std::vector<double> left( degree + 1, 0. );
  std::vector<double> right( degree + 1, 0. );
  // cчитаем полный треугольник от базисных функций по формуле:
  // N_(i,k)(x) = (x - u_i)/(u(i + k) - u_i) * N_(i, k-1)(x) + (u_(i+k) - x)/(u_(i+k+1) - u_(i+1))*N_(i+1, k-1)(x)  (2)
  for ( size_t j = 1; j <= degree; j++ )
  {
    left[j] = x - nodes[i + 1 - j]; // считаем числитель для первого слагаемого формулы (2)
    right[j] = nodes[i + j] - x;  // считаем числитель для второго слагаемого формулы (2)
    double saved = 0.0;
    for ( size_t r = 0; r < j; r++ )
    {
      triangleNodes[j][r] = right[r + 1] + left[j - r]; // считаем знаменатель для правого слагаемого
      double temp = triangleNodes[r][j - 1] / triangleNodes[j][r]; // счтаем частное знаменателей для текущей интерации по формуле (2)
      triangleNodes[r][j] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
    }
    triangleNodes[j][j] = saved; // отдельно обработываем случай диагонального элемента
  }

  // хранение двух наиболее исользуемых столбцов разностей интервалов
  std::vector<std::vector<double>> tempders;
  tempders.resize( 2 );
  for ( size_t k = 0; k < 2; k++ )
    tempders[k].resize( degree + 1, 0. );

  // инициализируем нулевыми проивводными массив производных
  for ( size_t j = 0; j <= degree; j++ )
    ders[0][j] = triangleNodes[j][degree];

  for ( int r = 0; r <= degree; r++ )
  {
    int s1 = 0; // первый столбец в массиве разностей интервалов
    int s2 = 1; // второй стобец в массиве разностей интервалов
    tempders[0][0] = 1.0;
    for ( int k = 1; k <= derivativeOrder; k++ )
    {
      double d = 0.0;
      int rk = r - k;
      int degk = degree - k;
      if ( r >= k )
      {
        tempders[s2][0] = tempders[s1][0] / triangleNodes[degk + 1][rk];
        d = tempders[s2][0] * triangleNodes[rk][degk];
      }
      int j1 = 0;
      if ( rk >= -1 )
        j1 = 1;
      else
        j1 = -rk;
      int j2 = 0;
      if ( r - 1 <= degk )
        j2 = k - 1;
      else
        j2 = degree - r;
      //
      for ( size_t j = j1; j <= j2; j++ )
      {
        tempders[s2][j] = ( tempders[s1][j] - tempders[s1][j - 1] ) /
          triangleNodes[degk + 1][rk + j];
        d += tempders[s2][j] * triangleNodes[rk + j][degk];
      }
      if (r <= degk)
      {
        tempders[s2][k] = -tempders[s1][k - 1] / triangleNodes[degk + 1][r];
        d += tempders[s2][k] * triangleNodes[r][degk];
      }
      ders[k][r] = d;
      // меняем местами столбцы
      std::swap( s1, s2 );
    }
  }

  // делим получившиеся значения на коэффициент перед квадратной скобкой в формуле (1)
  int r = degree;
  for ( int k = 1; k <= derivativeOrder; k++ )
  {
    for ( int j = 0; j <= degree; j++ )
      ders[k][j] = ders[k][j] * r;
    r = r * ( degree - k );
  }
}


//-----------------------------------------------------------------------------
//  Сдвиг по оси x на xShift, по оси y на yShift.
// ---
void NurbsCurve::Translate( double xShift, double yShift )
{
  for ( size_t i = 0; i < poles.size(); i++ )
    poles[i].Translate( xShift, yShift );

}


//-----------------------------------------------------------------------------
//  Повернуть относительно начала координат на угол alpha.
// ---
void NurbsCurve::Rotate( double alpha )
{
  for ( size_t i = 0; i < poles.size(); i++ )
    poles[i].Rotate( alpha );
}


//-----------------------------------------------------------------------------
//  Повернуть относительно начала координат на угол alpha.
// ---
void NurbsCurve::Scale(double XScaling, double YScaling)
{
  for ( size_t i = 0; i < poles.size(); i++ )
    poles[i].Scale( XScaling, YScaling );
}



//-----------------------------------------------------------------------------
//  Подсчитать значения базисных функций, умноженных на вес на отрезке x - degree.
// ---
double NurbsCurve::CountWeight( int k, double x )  const
{
  double w = 0.0;
  std::vector<double> node = BasicFunctions( k, x );

  for ( size_t i = 0; i <= degree; i++ )
  {
    w += node[i] * weights[k - degree  + i];
  }
  return w;
}


////-----------------------------------------------------------------------------
////  Подсчитать значения производных базисных функций, умноженных на вес на отрезке x - degree.
//// ---
//double NurbsCurve::CountWeightD( double t , int span)  const
//{
//  double w = 0.0;
//  std::vector<std::vector<double>> ders;
//  ComputeBasicFunctionD(t, span, 1, ders);
//  for ( size_t i = 0; i <= degree; i++ )
//  {
//    w += ders[1][i] * weights[span - degree + i];
//  }
//  return w;
//}


////-----------------------------------------------------------------------------
////  Подсчитать значения вторых производных базисных функций, умноженных на вес на отрезке x - degree.
//// ---
//double NurbsCurve::CountWeightD2( double t , int span)  const
//{
//  double w = 0.0;
//  std::vector<std::vector<double>> ders;
//  ComputeBasicFunctionD(t, span, 2, ders);
//  for ( size_t i = 0; i <= degree; i++ )
//  {

//    w += ders[2][i] * weights[span - degree + i];
//  }
//  return w;
//}


//-----------------------------------------------------------------------------
//   Вернуть точку на кривой по параметру t.
// ---
Point NurbsCurve::GetPoint( double t ) const
{
  if ( IsValid() )
  {
    double currentT = FixParametr(t);
    int span = FindSpan(currentT);
    double weightNurbs = CountWeight(span, currentT);
    Point resultPoint( 0.0, 0.0 );
    std::vector<double> node = BasicFunctions(span, currentT);

    for ( int i = 0; i <= degree; i++ )
    {
      resultPoint = resultPoint + poles[span - degree + i] * node[i] * weights[span - degree + i];
    }
    if ( fabs(weightNurbs) <CommonConstantsMath::NULL_TOL )
      return  Point( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
    else
      return Point( resultPoint * ( 1. / weightNurbs) );
  }
  else
    return
        Point( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}



//-----------------------------------------------------------------------------
//   Подсчитать значения базисных функций на отрезке x - degree.
// ---
std::vector<double> NurbsCurve::BasicFunctions( int i, double x) const
{
  // считаем ненулевые базисные функции по формуле : N_(i,k)(x) = (x - u_i)/(u(i + k) - u_i) * N_(i, k-1)(x) + (u_(i+k) - x)/(u_(i+k+1) - u_(i+1))*N_(i+1, k-1)(x)  (3)
  std::vector<double> N( degree + 1, 0. );
  std::vector<double> left( degree + 1, 0. );
  std::vector<double> right( degree + 1, 0. );
  // по определению, N_(i,0) = 1, если nodes[i] =< x =< nodes[i+1]
  // N_(i,0) = 0, иначе
  N[0] = 1.0;

  // считаем поочередно каждый слой треугольника базисных функций
  for ( size_t j = 1; j <= degree; j++ )
  {
    left[j] = x - nodes[i + 1 - j]; // числитель левого слагаемого в формуле (3)
    right[j] = nodes[i + j] - x;   // числитель правого слагаемого в формуле (3)
    double saved = 0.0;
    // счтаем поочередно все базисные функции на слое
    for ( size_t k = 0; k < j; k++ )
    {
      double temp = N[k] / ( right[k + 1] + left[j - k] );
      N[k] = saved + right[k + 1] * temp;
      saved = left[j - k] * temp;
    }
    N[j] = saved; // последнюю базисную функцию дописываем отдельно
  }
  return N;
}



//-----------------------------------------------------------------------------
//  Производные от базисных функций, помноженные на соответсвующие точки кривой.
// Возвращается ветор, такой что, vector[k] - это опорные точки ненулевого интервала, помноженые на вес, помноженые на k-ю производную базисных функций
// ---
std::vector<Point> NurbsCurve::PointDers( double t, int der, const std::vector<std::vector<double>>& ders ) const
{
  double tcurrent = FixParametr( t );
  // находим ненулевой промежуток, на котором определены базисные функции
  size_t span = FindSpan( tcurrent );
  std::vector<Point> points;

  // проходим по всему вектору производных
  for ( size_t j = 0; j <= der; j++ )
  {
    Point resultPoint;
    // для каждой производной проходим по ненулевому интервалу базисных функций, каждое значение базисной функции умножаем на опорную точку и вес
    for ( size_t i = 0; i <= degree; i++ )
    {
      resultPoint = resultPoint + poles[span - degree + i] * ders[j][i] * weights[span - degree + i];
    }
    points.push_back( resultPoint );
  }
  return points;
}



//-----------------------------------------------------------------------------
//  Производные от базисных функций, помноженные на соответсвующие веса кривой.
//  Возвращается ветор, такой что, vector[k] - веса, принадлежащие ненулевому интервалу, помноженые на k-ю производную базисных функций.
// ---
std::vector<double> NurbsCurve::WeightDers( double t, int der, const std::vector<std::vector<double>>& ders ) const
{
  double tcurrent = FixParametr( t );
  size_t span = FindSpan( tcurrent );
    // находим ненулевой промежуток, на котором определены базисные функции
  std::vector<double> result;
  // проходим по всему вектору производных
  for ( size_t j = 0; j <= der; j++ )
  {
    double resultWeight = 0;
     // для каждой производной проходим по ненулевому интервалу базисных функций, каждое значение базисной функции умножаем на опорную точку и вес
    for ( size_t i = 0; i <= degree; i++ )
    {
      resultWeight += weights[span - degree + i] * ders[j][i];
    }
  result.push_back( resultWeight );
  }
  return result;
}


//-----------------------------------------------------------------------------
// Внутренняя функция для посдчета производной порядка der.
// ---
Vector NurbsCurve::CountingDer( double t, int der) const
{
  double tcurrent = FixParametr( t );
  int span = FindSpan( tcurrent );
  // Считаем производные от базисных функций на интервале span - degree, span
  std::vector<std::vector<double>> ders;
  ComputeBasicFunctionD( tcurrent, span, der, ders );
  // Считаем производные от базисных функций, домноженных на веса
  std::vector<double> weight = WeightDers( tcurrent, der, ders );
   // Считаем производные от базисных функций, домноженных на веса и соотвествующие точки
  std::vector<Point> pointd = PointDers( t, der, ders );
  Point resultPoint;

  std::vector<Point> dtempders; // вектор для хранения производных до порядка der
  dtempders.resize( der  + 1 );
  for ( size_t k = 0; k <= der; k++ )
  {
    resultPoint = pointd[k];
    for ( size_t i = 1; i <= k; i++ )
      resultPoint = resultPoint - dtempders[k - i] * Bin( k, i ) * weight[i];
    dtempders[k] = resultPoint * ( 1 / weight[0] );
  }
  return Vector( dtempders[der].GetX(), dtempders[der].GetY() );
}


//-----------------------------------------------------------------------------
// Вернуть производную на nurbs по параметру t.
// ---
Vector  NurbsCurve::GetDerivative( double t ) const
{
  if ( IsValid() )
  {
    double currentT = FixParametr( t );
    return CountingDer( currentT, 1 );
  }
  else
    return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
//  Вернуть вторую производную на nurbs по параметру t.
// ---
Vector  NurbsCurve::Get2Derivative( double t ) const
{
  if ( IsValid() )
  {
    double currentT = FixParametr( t );
    return  CountingDer( currentT, 2 );
  }
  else
    return Vector( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}



//-----------------------------------------------------------------------------
//  Вернуть границы параметра для Nurbs : [nodes[0], nodes[size]].
// ---
Range  NurbsCurve::GetRange() const
{
  if ( IsValid() )
  {
    if ( isClosed )
      return Range( nodes[degree], nodes[nodes.size() - 1 - degree] );

    return Range( nodes[0], nodes[nodes.size() - 1] );
  }
  else
    return Range( std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() );
}


//-----------------------------------------------------------------------------
// Вернуть имя, используемое для записи примитива в файл.
// ---
std::string NurbsCurve::GetName() const
{
  return "Nurbs";
}


//-----------------------------------------------------------------------------
// Проверка на правильность кривой.
// ---
bool NurbsCurve::IsValid() const
{
  return ( poles.size() > 0 );
}


//-----------------------------------------------------------------------------
// Вернуть опорные точки.
// ---
std::vector<Point> NurbsCurve::GetPoles() const
{
  return poles;
}



//-----------------------------------------------------------------------------
//  Вернуть веса опорных точек.
// ---
std::vector<double> NurbsCurve::GetWeights() const
{
  return weights;
}


//-----------------------------------------------------------------------------
// Вернуть границы параметра t для базисных полиномов.
// ---
std::vector<double> NurbsCurve::GetNodes() const
{
  return nodes;
}


//-----------------------------------------------------------------------------
// Вернуть замкнутость Nurbs - кривой.
// ---
bool NurbsCurve::IsClosed() const
{
  return isClosed;
}


//-----------------------------------------------------------------------------
//  Вернуть степень Nurbs - кривой.
// ---
size_t NurbsCurve::Degree() const
{
  return degree;
}

//-----------------------------------------------------------------------------
//  Вернуть опорные точки, использованные для построения nurbs - кривой.
// ---
std::vector<Point> NurbsCurve::GetReferensedPoints() const
{
  return poles;
}
}
