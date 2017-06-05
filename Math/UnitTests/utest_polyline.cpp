//
// Created by alakhverdyants on 22.05.17.
//
#include "../GeomPolyline.h"
#include "gtest/gtest.h"
using namespace Math;
/// если две точки в полилинии, они не должны совпадать
TEST(PolylineTest, ConstructSamePoints)
{
	Point samePoint(0.0, 0.0);
	std::vector<Point> samePoints{ samePoint, samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// в полилинии всегда должно быть больше (или равно) двух точек
TEST( PolylineTest, ConstructOnePoint )
{
	Point samePoint( 0.0, 0.0 );
	std::vector<Point> samePoints{ samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// в полилинии всегда должно быть больше (или равно) двух точек
TEST(PolylineTest, Construct0Points)
{
	std::vector<Point> samePoints;
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE(polyline.IsValid());
}

/// точки ( кроме начала и конца не должны совпадать между собой
TEST(PolylineTest, ConstructSame4Point)
{
	Point samePoint( 0.0, 0.0 );
	std::vector<Point> samePoints{ samePoint, samePoint, samePoint, samePoint };
	const GeomPolyline polyline( samePoints );
	EXPECT_FALSE( polyline.IsValid() );
}
/// проверяем правильность получения точки
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

	/// если парметр заходит за границу, то возвращаем ближайшею точку
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
/// вторые производные всегда равны нулю
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

	polyline.Rotate( -3.14159265358979323846 / 4. );
	point = polyline.GetPoint( 0. );
	EXPECT_NEAR( point.GetX(), -1.4142135623730954, 1.e-7 );
	EXPECT_NEAR( point.GetY(), 0.29289321881345254, 1.e-7 );

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




