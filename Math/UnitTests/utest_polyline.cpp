//
// Created by alakhverdyants on 22.05.17.
//
#include "../GeomPolyline.h"
#include "gtest/gtest.h"
using namespace Math;
/// РµСЃР»Рё РґРІРµ С‚РѕС‡РєРё РІ РїРѕР»РёР»РёРЅРёРё, РѕРЅРё РЅРµ РґРѕР»Р¶РЅС‹ СЃРѕРІРїР°РґР°С‚СЊ
TEST(PolylineTest, ConstructSamePoints)
{
	Point samePoint(0.0, 0.0);
	std::vector<Point> samePoints{ samePoint, samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// РІ РїРѕР»РёР»РёРЅРёРё РІСЃРµРіРґР° РґРѕР»Р¶РЅРѕ Р±С‹С‚СЊ Р±РѕР»СЊС€Рµ (РёР»Рё СЂР°РІРЅРѕ) РґРІСѓС… С‚РѕС‡РµРє
TEST( PolylineTest, ConstructOnePoint )
{
	Point samePoint( 0.0, 0.0 );
	std::vector<Point> samePoints{ samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// РІ РїРѕР»РёР»РёРЅРёРё РІСЃРµРіРґР° РґРѕР»Р¶РЅРѕ Р±С‹С‚СЊ Р±РѕР»СЊС€Рµ (РёР»Рё СЂР°РІРЅРѕ) РґРІСѓС… С‚РѕС‡РµРє
TEST(PolylineTest, Construct0Points)
{
	std::vector<Point> samePoints;
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE(polyline.IsValid());
}

/// С‚РѕС‡РєРё ( РєСЂРѕРјРµ РЅР°С‡Р°Р»Р° Рё РєРѕРЅС†Р° РЅРµ РґРѕР»Р¶РЅС‹ СЃРѕРІРїР°РґР°С‚СЊ РјРµР¶РґСѓ СЃРѕР±РѕР№
TEST(PolylineTest, ConstructSame4Point)
{
	Point samePoint( 0.0, 0.0 );
	std::vector<Point> samePoints{ samePoint, samePoint, samePoint, samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// РїСЂРѕРІРµСЂСЏРµРј РїСЂР°РІРёР»СЊРЅРѕСЃС‚СЊ РїРѕР»СѓС‡РµРЅРёСЏ С‚РѕС‡РєРё
TEST(PolylineTest, GetPoint)
{
	std::vector<Point> points;
	points.push_back( Point(1., 1.) );
	points.push_back( Point(2., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 3.) );
	points.push_back( Point(2., 0.) );

	const GeomPolyline polyline( points );
	auto point = polyline.GetPoint( 0.0 );
	EXPECT_NEAR( point.GetX(), 1., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 1., 1.e-7 );

	point = polyline.GetPoint( 1. );
	EXPECT_NEAR( point.GetX(), 2., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 1., 1.e-7 );

	point = polyline.GetPoint( 0.5 );
	EXPECT_NEAR( point.GetX(), 1.5, 1.e-7 );
	EXPECT_NEAR( point.GetY(), 1.0, 1.e-7 );

	/// РµСЃР»Рё РїР°СЂРјРµС‚СЂ Р·Р°С…РѕРґРёС‚ Р·Р° РіСЂР°РЅРёС†Сѓ, С‚Рѕ РІРѕР·РІСЂР°С‰Р°РµРј Р±Р»РёР¶Р°Р№С€РµСЋ С‚РѕС‡РєСѓ
	point = polyline.GetPoint( -0.25 );
	EXPECT_NEAR( point.GetX(), 1., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 1., 1.e-7 );

}

TEST( PolylineTest, GetDerivativePoint )
{
	std::vector<Point> points;
	points.push_back( Point(1., 1.) );
	points.push_back( Point(2., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 3.) );
	points.push_back( Point(2., 0.) );

	const GeomPolyline polyline( points );
	auto der = polyline.GetDerivativePoint( 0.0 );
	EXPECT_NEAR( der.GetX(), 1., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.GetDerivativePoint( 0.25 );
	EXPECT_NEAR( der.GetX(), 1., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.GetDerivativePoint( -1.2 );
	EXPECT_NEAR( der.GetX(), 1., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.GetDerivativePoint( 2.3 );
	EXPECT_NEAR( der.GetX(), 3., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 1., 1.e-7 );
}
/// РІС‚РѕСЂС‹Рµ РїСЂРѕРёР·РІРѕРґРЅС‹Рµ РІСЃРµРіРґР° СЂР°РІРЅС‹ РЅСѓР»СЋ
TEST(PolylineTest, Get2DerivativePoint)
{
	std::vector<Point> points;
	points.push_back( Point(8., 1.) );
	points.push_back( Point(5., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 9.) );
	points.push_back( Point(2., 0.) );

	const GeomPolyline polyline( points );

	auto der = polyline.Get2DerivativePoint( 0.0 );
	EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.Get2DerivativePoint( 1.25 );
	EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.Get2DerivativePoint( -1.9999 );
	EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

	der = polyline.Get2DerivativePoint( 2.9 );
	EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
	EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
}

TEST(PolylineTest, GetRange)
{
	std::vector<Point> points;
	points.push_back( Point(8., 1.) );
	points.push_back( Point(5., 1.) );
	points.push_back( Point(10., 2.) );
	points.push_back( Point(4., 9.) );
	points.push_back( Point(2., 0.) );
	points.push_back( Point(5., 5.) );
	points.push_back( Point(6., 11.) );
	points.push_back( Point(7., 11.) );
	const GeomPolyline polyline( points );

	const auto range = polyline.GetRange();
	EXPECT_NEAR( range.GetStart(), 0., 1.e-7 );
	EXPECT_NEAR( range.GetEnd(), 8., 1.e-7 );
}

TEST(PolylineTest, GetAsPolyLine)
{

	std::vector<Point> points;
	points.push_back( Point(8., 1.) );
	points.push_back( Point(5., 1.) );
	points.push_back( Point(10., 2.) );
	points.push_back( Point(4., 9.) );
	points.push_back( Point(2., 0.) );
	points.push_back( Point(5., 5.) );
	points.push_back( Point(6., 11.) );
	points.push_back( Point(7., 11.) );
	const GeomPolyline polyline(points);
	 GeomPolyline polylineFromPolyline;
	polyline.GetAsPolyLine( polylineFromPolyline, 1.e-7 );
	EXPECT_EQ( polylineFromPolyline.GetReferensedPoints().size(), 8 );
	EXPECT_NEAR( polylineFromPolyline.GetReferensedPoints()[0].GetX(), 8., 1.e-7 );
	EXPECT_NEAR( polylineFromPolyline.GetReferensedPoints()[0].GetY(), 1., 1.e-7 );
	EXPECT_NEAR( polylineFromPolyline.GetReferensedPoints()[1].GetX(), 5., 1.e-7 );
	EXPECT_NEAR( polylineFromPolyline.GetReferensedPoints()[1].GetY(), 1., 1.e-7 );

}


TEST(PolylineTest, Translation)
{
	std::vector<Point> points;
	points.push_back( Point(1., 1.) );
	points.push_back( Point(2., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 3.) );
	points.push_back( Point(2., 0.) );

	GeomPolyline polyline( points );
	polyline.Translate( 1.0, 2.0 );
	auto point = polyline.GetPoint( 0. );
	EXPECT_NEAR(point.GetX(), 2.0, 1.e-7);
	EXPECT_NEAR(point.GetY(), 3.0, 1.e-7);

	point = polyline.GetPoint(1.);
	EXPECT_NEAR( point.GetX(), 3., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 3., 1.e-7 );
}

TEST(PolylineTest, Rotation)
{
	std::vector<Point> points;
	points.push_back( Point(1., 1.) );
	points.push_back( Point(2., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 3.) );
	points.push_back( Point(2., 0.) );
	GeomPolyline polyline(points);

	polyline.Rotate( 3.14159265358979323846 );
	auto point = polyline.GetPoint( 0. );
	EXPECT_NEAR( point.GetX(), -1., 1.e-7 );
	EXPECT_NEAR( point.GetY(), -1., 1.e-7 );

	point = polyline.GetPoint( 1. );
	EXPECT_NEAR( point.GetX(), -2., 1.e-7 );
	EXPECT_NEAR( point.GetY(), -1., 1.e-7 );

}

TEST(PolylineTest, Scaling)
{
	std::vector<Point> points;
	points.push_back( Point(1., 1.) );
	points.push_back( Point(2., 1.) );
	points.push_back( Point(1., 2.) );
	points.push_back( Point(4., 3.) );
	points.push_back( Point(2., 0.) );
	GeomPolyline polyline( points );

	polyline.Scale( 2., 0.5 );
	auto point = polyline.GetPoint( 0. );
	EXPECT_NEAR( point.GetX(), 2., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 0.5, 1.e-7 );

	point = polyline.GetPoint( 1. );
	EXPECT_NEAR( point.GetX(), 4., 1.e-7 );
	EXPECT_NEAR( point.GetY(), 0.5, 1.e-7 );
}


static const double pointNearValue( 1e-7 );

// Конструткор точки, функция нахождения расстояния
TEST( Point, DistancePoints )
{
  Point p1, p2( 3.0, -4.0 );
  // Тест конструктора по умолчанию
  ASSERT_TRUE( p1.IsValid() );
  ASSERT_TRUE( p2.IsValid() );

  ASSERT_NEAR( p1.GetX(), 0.0, pointNearValue );
  ASSERT_NEAR( p1.GetY(), 0.0, pointNearValue );
  // Тест конструктора по координатам
  ASSERT_NEAR( p2.GetX(), 3.0, pointNearValue );
  ASSERT_NEAR( p2.GetY(), -4.0, pointNearValue );
  // Расстояние от точки до себя
  ASSERT_NEAR( Math::Distance( p1, p1 ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( p2, p2 ), 0.0, pointNearValue );
  // Теорема Пифагора
  ASSERT_NEAR( Math::Distance( p1, p2 ), 5.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( p2, p1 ), 5.0, pointNearValue );
}


// Тесты на устройство полилинии
TEST( PolylineTest, PolylineConstitution )
{
  GeomPolyline polylineD;
  EXPECT_FALSE( polylineD.IsValid() ); // Полилинии по умолчанию невалидны

  std::vector<Point> points0;
  GeomPolyline polyline0( points0 );
  EXPECT_FALSE( polyline0.IsValid() ); // Полилинии без точек невалидны

  std::vector<Point> points1( 1 , Point() );
  GeomPolyline polyline1( points1 );
  EXPECT_FALSE( polyline1.IsValid() ); // Полилинии с одной точкой невалидны

  std::vector<Point> points2( 2, Point() );
  points2[0].Translate( 5.0, 10.0 );
  GeomPolyline polyline2( points2 );
  EXPECT_TRUE( polyline2.IsValid() ); // Полилинии с двумя разными точками разрешены

  std::vector<Point> points3( 4, Point() );
  points3[1].Translate( 5.0, 0.0 );
  points3[2].Translate( 5.0, 10.0 );
  points3[3].Translate( 0.0, 10.0 );
  GeomPolyline polyline3( points3 );
  EXPECT_TRUE( polyline3.IsValid() ); // Незамкнутые полилинии разрешены

  std::vector<Point> points4( 4, Point() );
  points4[1].Translate( 5.0, 0.0 );
  points4[2].Translate( 5.0, 10.0 );
  GeomPolyline polyline4( points4 );
  EXPECT_TRUE( polyline4.IsValid() ); // Замкнутые полилинии разрешены

  std::vector<Point> points5( 4, Point() );
  points5[1].Translate( 5.0, 10.0 );
  points5[2].Translate( 5.0, 10.0 );
  points5[3].Translate( 2.0, 30.0 );
  GeomPolyline polyline5( points5 );
  EXPECT_FALSE( polyline5.IsValid() ); // совпадающие внутренние точки недопустимы

  std::vector<Point> points6( 5, Point() );
  points6[1].Translate( 5.0, 10.0 );
  points6[2].Translate( 8.0, 10.0 );
  points6[3].Translate( 5.0, 10.0 );
  points6[4].Translate( 50.0, 100.0 );
  GeomPolyline polyline6( points6 );
  EXPECT_FALSE( polyline6.IsValid() ); // совпадающие внутренние точки недопустимы

  std::vector<Point> points7( 5, Point() );
  points7[1].Translate( 5.0, 10.0 );
  points7[2].Translate( 8.0, 10.0 );
  points7[3].Translate( 5.0, 10.0 );
  GeomPolyline polyline7( points7 );
  EXPECT_FALSE( polyline7.IsValid() ); // совпадающие внутренние точки недопустимы и для замкнутой полилинии

  // Проверить то же самое для метода Init
}


TEST( PolylineTest, CorrectInput )
{
  // Тестирование кривой, созданной по корректным данным в корректных операциях
  std::vector<Point> points;
  points.push_back( Point( 0., 0. ) ); // параметр 0.0
  points.push_back( Point( 2., 0. ) ); // параметр 1.0
  points.push_back( Point( 2., 1. ) ); // параметр 2.0
  points.push_back( Point( 0., 1. ) ); // параметр 3.0
  GeomPolyline polyline( points );
  ASSERT_TRUE( polyline.IsValid() );

  // Тестируем диапазон
  Range pRange = polyline.GetRange();
  EXPECT_GT( pRange.GetEnd(), pRange.GetStart() );
  EXPECT_NEAR( pRange.GetStart(), 0.0, pointNearValue );
  EXPECT_NEAR( pRange.GetEnd(), 3.0, pointNearValue );
  // преобразование парамтера в диапазон  
  EXPECT_NEAR( polyline.FixedRange( 2.0 ), 2.0, pointNearValue ); // Середина диапазона
  EXPECT_NEAR( polyline.FixedRange( pRange.GetStart() ), pRange.GetStart(), pointNearValue ); // Левая граница
  EXPECT_NEAR( polyline.FixedRange( pRange.GetEnd() ), pRange.GetEnd(), pointNearValue ); // Правая граница
  // За границами
  EXPECT_NEAR( polyline.FixedRange( pRange.GetStart() - 5.0 ), pRange.GetStart(), pointNearValue ); // Левая граница
  EXPECT_NEAR( polyline.FixedRange( pRange.GetEnd() + 2.0 ), pRange.GetEnd(), pointNearValue ); // Правая граница

  // Тест вычисления точек

  // В целочисленных координатах должны быть исходные точки
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 0.0 ), points[0] ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 1.0 ), points[1] ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 2.0 ), points[2] ), 0.0, pointNearValue );
  // промежуточные точки
  Point inter0 = points[0] * 0.2 + points[1] * 0.8; // параметр 0.8
  Point inter1 = points[1] * 0.2 + points[2] * 0.8; // параметр 1.8
  Point inter2 = points[2] * 0.2 + points[3] * 0.8; // параметр 2.8
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 0.8 ), inter0 ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 1.8 ), inter1 ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( 2.8 ), inter2 ), 0.0, pointNearValue );
  // Тестируем производные
  Vector v00 = polyline.GetDerivativePoint( 0.0 );
  Vector v10 = polyline.GetDerivativePoint( 1.0 );
  Vector v20 = polyline.GetDerivativePoint( 2.0 );

  Vector v08 = polyline.GetDerivativePoint( 0.8 );
  Vector v18 = polyline.GetDerivativePoint( 1.8 );
  Vector v28 = polyline.GetDerivativePoint( 2.8 );

  // производные в точках: абсолютное значение
  ASSERT_NEAR( v00.GetX(), 2.0, pointNearValue );
  ASSERT_NEAR( v00.GetY(), 0.0, pointNearValue );
  
  ASSERT_NEAR( v10.GetX(), 0.0, pointNearValue );
  ASSERT_NEAR( v10.GetY(), 1.0, pointNearValue );
  
  ASSERT_NEAR( v20.GetX(), -2.0, pointNearValue );
  ASSERT_NEAR( v20.GetY(), 0.0, pointNearValue );

  // Непрерывность производной справа
  ASSERT_NEAR( v00.GetX(), v08.GetX(), pointNearValue );
  ASSERT_NEAR( v00.GetY(), v08.GetY(), pointNearValue );
  
  ASSERT_NEAR( v10.GetX(), v18.GetX(), pointNearValue );
  ASSERT_NEAR( v10.GetY(), v18.GetY(), pointNearValue );

  ASSERT_NEAR( v20.GetX(), v28.GetX(), pointNearValue );
  ASSERT_NEAR( v20.GetY(), v28.GetY(), pointNearValue );

  // Пограничные параметры и их непротиворечивость
  ASSERT_NEAR( Math::Distance( points[0], polyline.GetPoint( pRange.GetStart() ) ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( points.back(), polyline.GetPoint( pRange.GetEnd() ) ), 0.0, pointNearValue );

  // Применение метода коррекции параметра при вычислении точки

  // Вычисление точки
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( polyline.GetRange().GetStart() - 1.7 ), polyline.GetPoint( polyline.GetRange().GetStart() ) ), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( polyline.GetPoint( polyline.GetRange().GetEnd()), polyline.GetPoint( polyline.GetRange().GetEnd() + 2.4 ) ), 0.0, pointNearValue );

  // Вычисление производной
  ASSERT_NEAR( Math::Distance( Point(polyline.GetDerivativePoint( polyline.GetRange().GetStart() - 1.7 )), Point( polyline.GetDerivativePoint( polyline.GetRange().GetStart() ) )), 0.0, pointNearValue );
  ASSERT_NEAR( Math::Distance( Point( polyline.GetDerivativePoint( polyline.GetRange().GetEnd() )), Point( polyline.GetDerivativePoint( polyline.GetRange().GetEnd() + 2.4 ) )), 0.0, pointNearValue );

  // Расстояние до точки
  ASSERT_NEAR( polyline.DistanceToPoint( Point() ), 0.0, pointNearValue );
  ASSERT_NEAR( polyline.DistanceToPoint( Point( 1.0, 0.0 ) ), 0.0, pointNearValue );
  ASSERT_NEAR( polyline.DistanceToPoint( Point( -1., 0. ) ), 1.0, pointNearValue );
}


// Тестирование погранчиных случаев

TEST( PolylineTest, ExtremeInput2 )
{
  // полилиния по двум точкам
}

TEST( PolylineTest, ExtremeInputClosed )
{
  // Замкнутая полилиния по трём точкам
}

// Тестирование "некорректных" случаев

TEST( PolylineTest, InvalidPolyline )
{
  // Тестирование невалидной полилинии
  GeomPolyline polyline;

  ASSERT_FALSE( polyline.IsValid() );
  Range emptyRange = polyline.GetRange();

  EXPECT_NEAR( emptyRange.GetStart(), emptyRange.GetEnd(), pointNearValue ); // Диапазон нулевой длины или ещё какой-то признак особого состояния

  const double midParam = 0.5 * (emptyRange.GetStart() + emptyRange.GetEnd());
  Point midPt = polyline.GetPoint( midParam );
  EXPECT_FALSE( midPt.IsValid() ); // вычисление точки должно формально работать, но нужно сигнализировать, что результатом нельзя пользоваться

  Vector midDer = polyline.GetDerivativePoint( midParam );
  EXPECT_FALSE( midDer.IsValid() ); // вычисление производной должно формально работать, но нужно сигнализировать, что результатом нельзя пользоваться

  GeomPolyline polyReplica;
  std::vector<Point> repInitPts( 2, Point() ) ;
  repInitPts[1].Translate( 10., 20. );

  polyline.GetAsPolyLine( polyReplica, Math::CommonConstantsMath::PRECISION_POLYLINE );
  EXPECT_FALSE( polyReplica.IsValid() ); // копия-полилиния должна насчитываться в любом случае, если не предусмотрено информации о невозможности это сделать

  double distanceTosomePt = polyline.DistanceToPoint( Point( 3., 4. ) ); // Должно как-то работать.
  EXPECT_LT( distanceTosomePt, -Math::CommonConstantsMath::NULL_TOL ); // В качестве критерия некорреткности операции предлагаю взять этот: возвращать, например, -1
}

// Тестирование переходов валидная - невалидная