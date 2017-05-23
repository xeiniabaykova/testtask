//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include <Point.h>
#include <Ellipse.h>


TEST(Circle, Construct0Point)
{
  std::vector<Point> wrongNumberPoints;
  const Ellipse ellipse( wrongNumberPoints );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Circle, Construct1Point)
{
  std::vector<Point> wrongNumberPoints {Point(0., 0.)};
  const Ellipse ellipse( wrongNumberPoints );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Circle, Construct2SamePoints)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints{samePoint, samePoint};
  const Ellipse ellipse( samePoints );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Circle, Construct3Points)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints{samePoint, Point(2.2, 3.7), samePoint};
  const Ellipse ellipse( samePoints );
  EXPECT_TRUE( ellipse.IsValid() );
}

TEST(Ellipse, Construct3PointsOnLine)
{
  Point point( 0.0, 0.0 );
  Point point1( 1.0, 1.0 );
  Point point2( 2.0, 2.0 );
  std::vector<Point> PointsOnLine { point, point1, point2 };
  const Ellipse ellipse( PointsOnLine );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, Construct3SamePoints)
{
  Point samePoint( 0.0, 0.0 );
  std::vector<Point> samePoints { samePoint, samePoint, samePoint };
  const Ellipse ellipse( samePoints );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, Construct3Points)
{
  std::vector<Point> points { Point(0., 0.), Point(1.2, 2.5), Point(0.5, 1.7) };
  const Ellipse ellipse( points );
  EXPECT_TRUE( ellipse.IsValid() );
}

TEST(Ellipse, Construct5SamePoints)
{
  Point samePoint( 0.0, 0.0 );
  std::vector<Point> samePoints { samePoint, samePoint, samePoint, samePoint, samePoint };
  const Ellipse ellipse( samePoints );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, Construct5PointsOnLine)
{
  std::vector<Point> points { Point(0.0, 0.0), Point(1.0, 1.0), Point(2.0, 2.0), Point(3.0, 3.0), Point(1.1, 2.4) };
  const Ellipse ellipse( points );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, Construct5Points)
{
  std::vector<Point> points { Point(0., 0.), Point(1.2, 2.5), Point(0.5, 1.7), Point(1.2, 2.5), Point(1.2, 2.5) };
  const Ellipse ellipse( points );
  EXPECT_TRUE( ellipse.IsValid() );
}
