//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include "../GeometricPoint.h"
#include "../Ellipse.h"


static bool IsEqualPoints( const Point & p1, const Point & p2, double eps=CommonConstantsMath::NULL_TOL )
{
  return fabs( p1.GetX() - p2.GetX() ) < eps && fabs( p1.GetY() - p2.GetY() ) < eps;
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
  EXPECT_FALSE( ellipse.IsValid() );
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
  circle.Translate( 2., -1. );
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
  circle.Rotate( CommonConstantsMath::PI/4. );
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
  circle.Scale( 2., 0.5 );
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
  EXPECT_TRUE( ellipse.IsValid() );
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
  std::vector<Point> points { Point(0.0, 0.0), Point(1.0, 1.0), Point(4.0, 4.0), Point(3.0, 3.0), Point(1.1, 2.4) };
  const Ellipse ellipse( points );
  EXPECT_TRUE( ellipse.IsValid() );
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

TEST(Ellipse, GetPoint)
{
  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
                             Point(2.7071067811865475, 1.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
                             Point(2.7071067811865475, 1.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(0.5, 3.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.), Point(0.5, 3.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(1.292893218813452, 0.29289321881345287)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.5, -1.5)) );
}

TEST(Ellipse, GetPoint2)
{
  std::vector<Point> points = { Point(2., 1.), Point(2.7071067811865475, 1.7071067811865475), Point(0.5, 3.5) };
  const Ellipse ellipse( points );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
                             Point(2.7071067811865475, 1.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
                             Point(2.7071067811865475, 1.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(0.5, 3.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.), Point(0.5, 3.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(1.292893218813452, 0.29289321881345287)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.5, -1.5)) );
}

TEST(Ellipse, GetRange)
{
  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  const auto range = ellipse.GetRange();
  EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( range.GetEnd(), CommonConstantsMath::PI*2., CommonConstantsMath::NULL_TOL );
}

TEST(Ellipse, GetDerivativePoint)
{
  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(0.), Point(-2.82842712474619, 2.8284271247461903)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(CommonConstantsMath::PI*2.),
                             Point(-2.82842712474619, 2.8284271247461903)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(CommonConstantsMath::PI/4.), Point(-2.5, 1.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(CommonConstantsMath::PI*9./4.), Point(-2.5, 1.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(CommonConstantsMath::PI),
                             Point(2.82842712474619, -2.8284271247461903)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetDerivativePoint(CommonConstantsMath::PI*5./4.), Point(2.5, -1.5)) );
}

TEST(Ellipse, Get2DerivativePoint)
{
  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(0.), Point(-0.7071067811865476, -0.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(CommonConstantsMath::PI*2.),
                             Point(-0.7071067811865476, -0.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(CommonConstantsMath::PI/4.), Point(1.5, -2.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(CommonConstantsMath::PI*9./4.), Point(1.5, -2.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(CommonConstantsMath::PI),
                             Point(0.7071067811865479, 0.7071067811865471)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.Get2DerivativePoint(CommonConstantsMath::PI*5./4.), Point(-1.5, 2.5)) );
}

TEST(Ellipse, GetAsPolyLine)
{
  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  std::vector<Point> points1, points2;
  ellipse.GetAsPolyLine( points1, 1.e-4 );
  ellipse.GetAsPolyLine( points2, CommonConstantsMath::PI/60. );
  EXPECT_TRUE( points1.size() > points2.size() );
  // Примитивная проверка, что расстояние между соседними точками зависит от кривизны участка кривой, к которому они
  // относятся.
  const  auto dist1 = DistancePointPoint(points1[0], points1[1]);
  const  auto dist2 = DistancePointPoint(points1[points1.size()/4], points1[points1.size()/4 + 1]);
  EXPECT_TRUE( 1.1 * dist1 < dist2 );
}

TEST(Ellipse, DistanceToPoint)
{
  // Проверка, что для окружности работает частный случай, когда расстояние может быть найдено точно.
  const Ellipse ellipse( Point(2., 1.), 1., 3., 0. );
  Point p( 0., 1. );
  EXPECT_NEAR( ellipse.DistanceToPoint(p), 1., 1.e-3 );
  p = Point( 2., 1. );
  EXPECT_NEAR( ellipse.DistanceToPoint(p), 1., 1.e-3 );
  p = Point( 2., 4. );
  EXPECT_NEAR( ellipse.DistanceToPoint(p), 0., 1.e-3 );
  p = Point( 2., 3.99 );
  EXPECT_NEAR( ellipse.DistanceToPoint(p), 0.01, 1.e-3 );
}

TEST(Ellipse, Translation)
{
  Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
  ellipse.Translate( 2., -1. );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
                             Point(4.7071067811865475, 0.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
                             Point(4.7071067811865475, 0.7071067811865475)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(2.5, 2.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.), Point(2.5, 2.5)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI),
                             Point(3.292893218813452, 0.29289321881345287-1.)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(5.5, -2.5)) );
}

TEST(Ellipse, Rotation)
{
  Ellipse ellipse( Point(0., 0.), 1., 4., CommonConstantsMath::PI/4. );
  ellipse.Rotate( -CommonConstantsMath::PI/4. );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.), Point(1., 0.)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.), Point(1., 0.)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.),
                             Point(0.7071067811865476, 2.82842712474619)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.),
                             Point(0.7071067811865476, 2.82842712474619)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(-1., 0.)) );
  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.),
                             Point(-0.7071067811865477, -2.82842712474619)) );
}

TEST(Ellipse, Scaling)
{
  Ellipse ellipse( Point(2., 1.), 1., 4., 0. );
  ellipse.Scale( 2., 0.5 );
  auto diam = DistancePointPoint( ellipse.GetPoint(0.), ellipse.GetPoint(CommonConstantsMath::PI) );
  EXPECT_NEAR( diam, 4., CommonConstantsMath::NULL_TOL );
  diam = DistancePointPoint( ellipse.GetPoint(CommonConstantsMath::PI/2.),
                             ellipse.GetPoint(CommonConstantsMath::PI*3./2.) );
  EXPECT_NEAR( diam, 4., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( ellipse.GetMajorRadius(), 2., CommonConstantsMath::NULL_TOL );
  EXPECT_NEAR( ellipse.GetMinorRadius(), 2., CommonConstantsMath::NULL_TOL );
}

TEST(Ellipse, IsValid)
{
  Ellipse ellipse(Point(2., 1.), 1., 4., 0.);
  EXPECT_TRUE( ellipse.IsValid() );
  ellipse.Scale( 2., 0.2 );
  EXPECT_TRUE( ellipse.IsValid() );
  ellipse.Scale( 1., 0 );
  EXPECT_FALSE( ellipse.IsValid() );
}

TEST(Ellipse, IsValid2)
{
  Ellipse ellipse(Point(2., 1.), 1., 0., 0.);
  EXPECT_FALSE( ellipse.IsValid() );
  Ellipse ellipse2(Point(2., 1.), 1., 1., 0.);
  EXPECT_TRUE( ellipse2.IsValid() );
}
