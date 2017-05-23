//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include "../Point.h"
#include "../Ellipse.h""
#include <CommonConstantsMath.h>


static bool IsEqualPoints( const Point & p1, const Point & p2, double eps=CommonConstantsMath::NULL_TOL )
{
Point point( 0.0, 0.0 );
Point point1( 1.0, 1.0 );
Point point2( 3.0, 3.0 );
std::vector<Point> PointsOnLine;
PointsOnLine.push_back( point );
PointsOnLine.push_back( point1 );
PointsOnLine.push_back( point2 );

Ellipse* ellipse = new Ellipse( PointsOnLine );
ASSERT_FALSE(ellipse->IsValid());
delete ellipse;

Point samePoint( 0.0, 0.0 );
std::vector<Point> samePoints;
samePoints.push_back( samePoint );
samePoints.push_back( samePoint );
samePoints.push_back( samePoint );
Ellipse* ellipse2 = new Ellipse( samePoints );
ASSERT_FALSE( ellipse2->IsValid() );
delete ellipse2;

std::vector<Point> wrongNumberPoints;
Ellipse* ellipse3 = new Ellipse(samePoints);
ASSERT_FALSE(ellipse3->IsValid());
delete ellipse3;

std::vector<Point> points;
Point pointCenter( 0.0, 0.0 );
Point pointr1( 1.2, 2.5 );
Point pointOnEllipse( 0.5, 1.7 );
points.push_back( pointCenter );
points.push_back( pointr1 );
points.push_back(pointOnEllipse);
Ellipse* ellipse4 = new Ellipse( points );
ASSERT_TRUE( ellipse4->IsValid() );
EXPECT_NEAR(ellipse4->GetPoint(0).GetX(), 1.1999999999999995, 1e-7);
EXPECT_NEAR(ellipse4->GetPoint(0).GetY(), 2.5000000000000000, 1e-7);

EXPECT_NEAR(ellipse4->GetDerivativePoint(0).GetX(), 0.0 , 1e-7);
EXPECT_NEAR(ellipse4->GetDerivativePoint(0).GetY(), 0.36709825739075547, 1e-7);

EXPECT_NEAR(ellipse4->Get2DerivativePoint(0).GetX(), -2.7730849247724092, 1e-7);
EXPECT_NEAR(ellipse4->Get2DerivativePoint(0).GetY(), 0.0, 1e-7);
delete ellipse4;
}

double DistancePointPoint( const Point & p1, const Point & p2 )
{
  return sqrt( (p1.GetX()-p2.GetX())*(p1.GetX()-p2.GetX()) + (p1.GetY()-p2.GetY())*(p1.GetY()-p2.GetY()) );
}

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

TEST(Circle, ConstructCRRA)
{
  const Ellipse circle( Point(1., 1.), 0, 1., CommonConstantsMath::PI/4. );
  EXPECT_FALSE( circle.IsValid() );
}

TEST(Circle, ConstructCRRA2)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  EXPECT_TRUE( circle.IsValid() );
}

TEST(Circle, GetPoint)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(3., 1.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(3., 1.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(1.+sqrt(2.), 1.+sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9./4.), Point(1.+sqrt(2.), 1.+sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(-1., 1.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(1.-sqrt(2.), 1.-sqrt(2.))) );
}

TEST(Circle, GetRange)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  const auto range = circle.GetRange();
  EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( range.GetEnd(), CommonConstantsMath::PI*2., CommonConstantsMath::NULL_TOL );
}

TEST(Circle, GetDerivativePoint)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(0.), Point(0., 2.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(CommonConstantsMath::PI*2.), Point(0., 2.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(CommonConstantsMath::PI/4.), Point(-sqrt(2.), sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(CommonConstantsMath::PI*9./4.), Point(-sqrt(2.), sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(CommonConstantsMath::PI), Point(0., -2.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetDerivativePoint(CommonConstantsMath::PI*5./4.), Point(sqrt(2.), -sqrt(2.))) );
}

TEST(Circle, Get2DerivativePoint)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(0.), Point(-2., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(CommonConstantsMath::PI*2.), Point(-2., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(CommonConstantsMath::PI/4.), Point(-sqrt(2.), -sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(CommonConstantsMath::PI*9./4.), Point(-sqrt(2.), -sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(CommonConstantsMath::PI), Point(2., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.Get2DerivativePoint(CommonConstantsMath::PI*5./4.), Point(sqrt(2.), sqrt(2.))) );
}

TEST(Circle, GetAsPolyLine)
{
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  std::vector<Point> points1, points2;
  circle.GetAsPolyLine( points1, CommonConstantsMath::PI/180. );
  circle.GetAsPolyLine( points1, CommonConstantsMath::PI/180. );  // специально 2 раза, чтобы проверить, что массив очищается
  circle.GetAsPolyLine( points2, CommonConstantsMath::PI/60. );
  EXPECT_EQ( points1.size()-1., 3.*(points2.size()-1.) );  // вычитаю 1 из-за замкнутости (дважды добавляется последняя точка)
}

TEST(Circle, DistanceToPoint)
{
  // Проверка, что для окружности работает частный случай, когда расстояние может быть найдено точно.
  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
  Point p( 0., 0. );
  EXPECT_NEAR( circle.DistanceToPoint(p), 2.-sqrt(2.), CommonConstantsMath::NULL_TOL );
  p = Point( 1., 1. );
  EXPECT_NEAR( circle.DistanceToPoint(p), 2., CommonConstantsMath::NULL_TOL );
  p = Point( 1.-sqrt(2.), 1.-sqrt(2.) );
  EXPECT_NEAR( circle.DistanceToPoint(p), 0, CommonConstantsMath::NULL_TOL );
  p = Point( 5., 5. );
  EXPECT_NEAR( circle.DistanceToPoint(p), sqrt(32.)-2., CommonConstantsMath::NULL_TOL );
}

TEST(Circle, Translation)
{
  Ellipse circle( Point(1., 1.), 2., 2., 0. );
  circle.Translation( 2., -1. );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(5., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(5., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(3.+sqrt(2.), sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9./4.), Point(3.+sqrt(2.), sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(1., 0.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.-sqrt(2.), -sqrt(2.))) );
}

TEST(Circle, Rotation)
{
  Ellipse circle(Point(1., 1.), 2., 2., 0.);
  circle.Rotation( CommonConstantsMath::PI/4. );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(1.+sqrt(2.), 1.+sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(1.+sqrt(2.), 1.+sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(1., 3.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9./4.), Point(1., 3.)) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(1.-sqrt(2.), 1.-sqrt(2.))) );
  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(1., -1.)) );
}

TEST(Circle, Scaling)
{
  Ellipse circle(Point(1., 1.), 2., 2., 0.);
  circle.Scaling( 2., 0.5 );
  auto diam = DistancePointPoint( circle.GetPoint(0.), circle.GetPoint(CommonConstantsMath::PI) );
  EXPECT_NEAR( diam, 8., CommonConstantsMath::NULL_TOL );
  diam = DistancePointPoint( circle.GetPoint(CommonConstantsMath::PI/2.),
                             circle.GetPoint(CommonConstantsMath::PI*3./2.) );
  EXPECT_NEAR( diam, 2., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( circle.GetMajorRadius(), 4., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( circle.GetMinorRadius(), 1., CommonConstantsMath::NULL_TOL );
}

TEST(Ellipse, Construct3PointsOnLine)
{
  std::vector<Point> PointsOnLine { Point(0.0, 0.0), Point(1.0, 1.0), Point(2.0, 2.0) };
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

TEST(Ellipse, ConstructCRRA)
{
  const Ellipse ellipse( Point(1., 1.), 0, 1., CommonConstantsMath::PI/4. );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, ConstructCRRA2)
{
  const Ellipse ellipse( Point(1., 1.), 2., 3., 0. );
  EXPECT_TRUE( ellipse.IsValid() );
}
