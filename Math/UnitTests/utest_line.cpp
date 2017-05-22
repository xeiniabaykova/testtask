//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include <Point.h>
#include <Line.h>
#include <memory>


TEST(Line, ConstructSamePoints)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints {samePoint, samePoint};
  const Line line( samePoints );
  ASSERT_FALSE( line.IsValid() );
}

TEST(Line, ConstructOnePoint)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints {samePoint};
  const Line line( samePoints );
  ASSERT_FALSE( line.IsValid() );
}

TEST(Line, ConstructThreePoint)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints {samePoint, Point(1.0, 0), samePoint};
  const Line line( samePoints );
  ASSERT_TRUE( line.IsValid() );
}

TEST(Line, GetPoint)
{
  const Line line( Point(0., 0.), Point(5., 5.) );
  auto point = line.GetPoint( 0.0 );
  EXPECT_NEAR( point.GetX(), 0., 1.e-7 );
  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );

  point = line.GetPoint( 1. );
  EXPECT_NEAR( point.GetX(), 5., 1.e-7 );
  EXPECT_NEAR( point.GetY(), 5., 1.e-7 );

  point = line.GetPoint( 0.5 );
  EXPECT_NEAR( point.GetX(), 2.5, 1.e-7 );
  EXPECT_NEAR( point.GetY(), 2.5, 1.e-7 );

  point = line.GetPoint( -0.25 );
  EXPECT_NEAR( point.GetX(), -1.25, 1.e-7 );
  EXPECT_NEAR( point.GetY(), -1.25, 1.e-7 );

  point = line.GetPoint( 1.75 );
  EXPECT_NEAR( point.GetX(), 8.75, 1.e-7 );
  EXPECT_NEAR( point.GetY(), 8.75, 1.e-7 );
}

TEST(Line, GetDerivativePoint)
{
  const Line line( Point(0., 0.), Point(5., 5.) );
  auto der = line.GetDerivativePoint( 0.0 );
  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );

  der = line.GetDerivativePoint( 0.25 );
  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );

  der = line.GetDerivativePoint( -1.2 );
  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );

  der = line.GetDerivativePoint( 2.3 );
  EXPECT_NEAR( der.GetX(), 5., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 5., 1.e-7 );
}

TEST(Line, Get2DerivativePoint)
{
  const Line line( Point(0., 0.), Point(5., 5.) );
  auto der = line.Get2DerivativePoint( 0.0 );
  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

  der = line.Get2DerivativePoint( 0.25 );
  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

  der = line.Get2DerivativePoint( -1.2 );
  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );

  der = line.Get2DerivativePoint( 2.3 );
  EXPECT_NEAR( der.GetX(), 0., 1.e-7 );
  EXPECT_NEAR( der.GetY(), 0., 1.e-7 );
}

TEST(Line, GetRange)
{
  const Line line( Point(0., 0.), Point(5., 5.) );
  const auto range = line.GetRange();
  EXPECT_NEAR( range.GetStart(), 0., 1.e-7 );
  EXPECT_NEAR( range.GetEnd(), 1., 1.e-7 );
}

TEST(Line, GetAsPolyLine)
{
  const Line line( Point(0., 0.), Point(5., 5.) );
  std::vector<Point> points;
  line.GetAsPolyLine( points, 1.e-7 );
  EXPECT_EQ( points.size(), 2 );
  EXPECT_NEAR( points[0].GetX(), 0., 1.e-7 );
  EXPECT_NEAR( points[0].GetY(), 0., 1.e-7 );
  EXPECT_NEAR( points[1].GetX(), 5., 1.e-7 );
  EXPECT_NEAR( points[1].GetY(), 5., 1.e-7 );

  line.GetAsPolyLine( points, 1.e-2 );
  EXPECT_EQ( points.size(), 2 );
  EXPECT_NEAR( points[0].GetX(), 0., 1.e-7 );
  EXPECT_NEAR( points[0].GetY(), 0., 1.e-7 );
  EXPECT_NEAR( points[1].GetX(), 5., 1.e-7 );
  EXPECT_NEAR( points[1].GetY(), 5., 1.e-7 );
}

TEST(Line, DistanceToPoint)
{
  const Line line( Point(0., 0.), Point(5., 0.) );
  Point point( -1., 1. );
  EXPECT_NEAR( line.DistanceToPoint(point), ::sqrt(2.), 1.e-7 );

  point = Point(2.3, 1.);
  EXPECT_NEAR( line.DistanceToPoint(point), 1., 1.e-7 );

  point = Point(7.5, 1.);
  EXPECT_NEAR( line.DistanceToPoint(point), ::sqrt(7.25), 1.e-7 );

  const Line line2( Point(0., 0.), Point(5., 5.) );
  point = Point( 6., -2. );
  EXPECT_NEAR( line2.DistanceToPoint(point), ::sqrt(32.), 1.e-7 );
}

TEST(Line, Translation)
{
  Line line( Point(0., 0.), Point(5., 5.) );
  line.Translation( 2.3, -1.7 );
  auto point = line.GetPoint( 0. );
  EXPECT_NEAR( point.GetX(), 2.3, 1.e-7 );
  EXPECT_NEAR( point.GetY(), -1.7, 1.e-7 );

  point = line.GetPoint( 1. );
  EXPECT_NEAR( point.GetX(), 7.3, 1.e-7 );
  EXPECT_NEAR( point.GetY(), 3.3, 1.e-7 );
}

TEST(Line, Rotation)
{
  Line line( Point(1., 1.), Point(5., 5.) );
  line.Rotation( 3.14159265358979323846 );
  auto point = line.GetPoint( 0. );
  EXPECT_NEAR( point.GetX(), -1., 1.e-7 );
  EXPECT_NEAR( point.GetY(), -1., 1.e-7 );

  point = line.GetPoint( 1. );
  EXPECT_NEAR( point.GetX(), -5., 1.e-7 );
  EXPECT_NEAR( point.GetY(), -5., 1.e-7 );

  line.Rotation( -3.14159265358979323846/4. );
  point = line.GetPoint( 0. );
  EXPECT_NEAR( point.GetX(), -::sqrt(2.), 1.e-7 );
  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );

  point = line.GetPoint( 1. );
  EXPECT_NEAR( point.GetX(), -::sqrt(50.), 1.e-7 );
  EXPECT_NEAR( point.GetY(), 0., 1.e-7 );
}

TEST(Line, Dilatation)
{
  Line line( Point(2., 1.), Point(6., 5.) );
  line.Dilatation( 2., 0.5 );
  auto point = line.GetPoint( 0. );
  EXPECT_NEAR( point.GetX(), 4., 1.e-7 );
  EXPECT_NEAR( point.GetY(), 0.5, 1.e-7 );

  point = line.GetPoint( 1. );
  EXPECT_NEAR( point.GetX(), 12., 1.e-7 );
  EXPECT_NEAR( point.GetY(), 2.5, 1.e-7 );
}
