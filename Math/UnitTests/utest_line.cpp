//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include "../Point.h"
#include "../Line.h"
#include "GeomPolyline.h"
#include <memory>

using namespace Math;
//TEST(Line, ConstructSamePoints)
//{
//  Point samePoint(0.0, 0.0);
//  std::vector<Point> samePoints {samePoint, samePoint};
//  const Line line( samePoints );
//  EXPECT_FALSE( line.IsValid() );
//}
//
//TEST(Line, ConstructOnePoint)
//{
//  Point samePoint(0.0, 0.0);
//  std::vector<Point> samePoints {samePoint};
//  const Line line( samePoints );
//  EXPECT_FALSE( line.IsValid() );
//}
//
//TEST(Line, Construct0Points)
//{
//  std::vector<Point> samePoints;
//  const Line line( samePoints );
//  EXPECT_FALSE( line.IsValid() );
//}
//
//
//TEST(Line, Construct4Point)
//{
//  Point samePoint(0.0, 0.0);
//  std::vector<Point> samePoints {samePoint, Point(1.0, 0), samePoint, samePoint};
//  const Line line( samePoints );
//  EXPECT_TRUE( line.IsValid() );
//}
//
//TEST(Line, GetPoint)
//{
//  const Line line( Point(0., 0.), Point(5., 5.) );
//  auto point = line.GetPoint( 0.0 );
//  EXPECT_NEAR( point.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 5., 1.e-7 );
//
//  point = line.GetPoint( 0.5 );
//  EXPECT_NEAR( point.GetX(), 2.5, 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 2.5, 1.e-7 );
//
//  point = line.GetPoint( -0.25 );
//  EXPECT_NEAR( point.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//
//  point = line.GetPoint( 1.75 );
//  EXPECT_NEAR( point.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 5., 1.e-7 );
//}
//
//TEST(Line, GetDerivative)
//{
//  const Line line( Point(0., 0.), Point(5., 5.) );
//  auto der = line.GetDerivative( 0.0 );
//  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );
//
//  der = line.GetDerivative( 0.25 );
//  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );
//
//  der = line.GetDerivative( -1.2 );
//  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );
//
//  der = line.GetDerivative( 2.3 );
//  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );
//}
//
//TEST(Line, Get2Derivative)
//{
//  const Line line( Point(0., 0.), Point(5., 5.) );
//  auto der = line.Get2Derivative( 0.0 );
//  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
//
//  der = line.Get2Derivative( 0.25 );
//  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
//
//  der = line.Get2Derivative( -1.2 );
//  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
//
//  der = line.Get2Derivative( 2.3 );
//  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
//}
//
//TEST(Line, GetRange)
//{
//  const Line line( Point(0., 0.), Point(5., 5.) );
//  const auto range = line.GetRange();
//  EXPECT_NEAR( range.GetStart(), 0., 1.e-7 );
//  EXPECT_NEAR( range.GetEnd(), 1., 1.e-7 );
//}
//
//TEST(Line, GetAsPolyLine)
//{
//  const Line line( Point(0., 0.), Point(5., 5.) );
//  GeomPolyline polyline;
//  line.GetAsPolyLine( polyline, 1.e-7 );
//  EXPECT_EQ( polyline.GetReferensedPoints().size(), 2 );
//  EXPECT_NEAR(polyline.GetReferensedPoints()[0].GetX(), 0., 1.e-7 );
//  EXPECT_NEAR(polyline.GetReferensedPoints()[0].GetY(), 0., 1.e-7 );
//  EXPECT_NEAR(polyline.GetReferensedPoints()[1].GetX(), 5., 1.e-7 );
//  EXPECT_NEAR(polyline.GetReferensedPoints()[1].GetY(), 5., 1.e-7 );
//
//  line.GetAsPolyLine( polyline, 1.e-2 );
//  EXPECT_EQ(polyline.GetReferensedPoints().size(), 2 );
//  EXPECT_NEAR( polyline.GetReferensedPoints()[0].GetX(), 0., 1.e-7 );
//  EXPECT_NEAR( polyline.GetReferensedPoints()[0].GetY(), 0., 1.e-7 );
//  EXPECT_NEAR( polyline.GetReferensedPoints()[1].GetX(), 5., 1.e-7 );
//  EXPECT_NEAR( polyline.GetReferensedPoints()[1].GetY(), 5., 1.e-7 );
//}
//
//TEST(Line, DistanceToPoint)
//{
// /* const Line line( Point(0., 0.), Point(5., 0.) );
//  Point point( -1., 1. );
//  EXPECT_NEAR( line.DistanceToPoint(point), ::sqrt(2.), 1.e-7 );
//
//  point = Point(2.3, 1.);
//  EXPECT_NEAR( line.DistanceToPoint(point), 1., 1.e-7 );
//
//  point = Point(7.5, 1.);
//  EXPECT_NEAR( line.DistanceToPoint(point), ::sqrt(7.25), 1.e-7 );
//
//  const Line line2( Point(0., 0.), Point(5., 5.) );
//  point = Point( 6., -2. );
//  EXPECT_NEAR( line2.DistanceToPoint(point), ::sqrt(32.), 1.e-7 );*/
//}
//
//TEST(Line, Translation)
//{
//  Line line( Point(0., 0.), Point(5., 5.) );
//  line.Translate( 2.3, -1.7 );
//  auto point = line.GetPoint( 0. );
//  EXPECT_NEAR( point.GetX(), 2.3, 1.e-7 );
//  EXPECT_NEAR( point.GetY(), -1.7, 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), 7.3, 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 3.3, 1.e-7 );
//}
//
//TEST(Line, Rotation)
//{
//  Line line( Point(1., 1.), Point(5., 5.) );
//  line.Rotate( 3.14159265358979323846 );
//  auto point = line.GetPoint( 0. );
//   EXPECT_NEAR( point.GetX(), -1., 1.e-7 );
//   EXPECT_NEAR( point.GetY(), -1., 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), -5., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), -5., 1.e-7 );
//
//  line.Rotate( -3.14159265358979323846/4. );
//  point = line.GetPoint( 0. );
//  EXPECT_NEAR( point.GetX(), -::sqrt(2.), 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), -::sqrt(50.), 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//}
//
//TEST(Line, Scaling)
//{
//  Line line( Point(2., 1.), Point(6., 5.) );
//  line.Scale( 2., 0.5 );
//  auto point = line.GetPoint( 0. );
//  EXPECT_NEAR( point.GetX(), 4., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0.5, 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), 12., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 2.5, 1.e-7 );
//}
//
//TEST(Line, IsValid)
//{
//  Line line( Point(2., 1.), Point(2., -3.) );
//  EXPECT_TRUE( line.IsValid() );
//  line.Scale( 1., 0.);
//  auto point = line.GetPoint( 0. );
//  EXPECT_NEAR( point.GetX(), 2., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//
//  point = line.GetPoint( 1. );
//  EXPECT_NEAR( point.GetX(), 2., 1.e-7 );
//  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
//
//  EXPECT_FALSE( line.IsValid() );
//}
//
//TEST(Line, IsValid2)
//{
//  Line line1( Point(2., 1.), Point(2., 1.) );
//  EXPECT_FALSE( line1.IsValid() );
//
//  Line line2( Point(2., 1.), Point(3., 1.) );
//  EXPECT_TRUE( line2.IsValid() );
//}
