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
  std::shared_ptr<Line> line = std::make_shared<Line>( samePoints );
  ASSERT_FALSE( line->IsValid() );
}

TEST(Line, ConstructOnePoint)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints {samePoint};
  std::shared_ptr<Line> line = std::make_shared<Line>( samePoints );
  ASSERT_FALSE( line->IsValid() );
}

TEST(Line, ConstructThreePoint)
{
  Point samePoint(0.0, 0.0);
  std::vector<Point> samePoints {samePoint, Point(1.0, 0), samePoint};
  std::shared_ptr<Line> line = std::make_shared<Line>( samePoints );
  ASSERT_TRUE( line->IsValid() );
}
