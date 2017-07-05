//
// Created by alakhverdyants on 22.05.17.
//
#include <gtest/gtest.h>
#include "../Point.h"
#include "../Ellipse.h"
#include "GeomPolyline.h"
using namespace Math;

static bool IsEqualPoints( const Point & p1, const Point & p2, double eps=CommonConstantsMath::NULL_TOL )
{
  return fabs( p1.GetX() - p2.GetX() ) < eps && fabs( p1.GetY() - p2.GetY() ) < eps;
}

static bool IsEqualVectors( const Vector & v1, const Vector & v2, double eps=CommonConstantsMath::NULL_TOL )
{
  return fabs( v1.GetX() - v2.GetX() ) < eps && fabs( v1.GetY() - v2.GetY() ) < eps;
}

double DistancePointPoint( const Point & p1, const Point & p2 )
{
  return sqrt( (p1.GetX()-p2.GetX())*(p1.GetX()-p2.GetX()) + (p1.GetY()-p2.GetY())*(p1.GetY()-p2.GetY()) );
}
//
//TEST(Circle, Construct0Point)
//{
//  std::vector<Point> wrongNumberPoints;
//  const Ellipse ellipse( wrongNumberPoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Circle, Construct1Point)
//{
//  std::vector<Point> wrongNumberPoints {Point(0., 0.)};
//  const Ellipse ellipse( wrongNumberPoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Circle, Construct2SamePoints)
//{
//  Point samePoint(0.0, 0.0);
//  std::vector<Point> samePoints{samePoint, samePoint};
//  const Ellipse ellipse( samePoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Circle, Construct3Points)
//{
//  Point samePoint(0.0, 0.0);
//  std::vector<Point> samePoints{samePoint, Point(2.2, 3.7), samePoint};
//  const Ellipse ellipse( samePoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Circle, ConstructCRRA)
//{
//  const Ellipse circle( Point(1., 1.), 0, 1., CommonConstantsMath::PI/4. );
//  EXPECT_FALSE( circle.IsValid() );
//}
//
//TEST(Circle, ConstructCRRA2)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  EXPECT_TRUE( circle.IsValid() );
//}
//
//TEST(Circle, GetPoint)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(3., 1.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(3., 1.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(1.+sqrt(2.), 1.+sqrt(2.))) );
//  EXPECT_TRUE(IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9. / 4.), Point(1. + sqrt(2.), 1. + sqrt(2.))));
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(-1., 1.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(1.-sqrt(2.), 1.-sqrt(2.))) );
//}
//
//TEST(Circle, GetRange)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  const auto range = circle.GetRange();
//  EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( range.GetEnd(), CommonConstantsMath::PI*2., CommonConstantsMath::NULL_TOL );
//}
//
//TEST(Circle, GetDerivative)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  EXPECT_TRUE( IsEqualVectors(circle.GetDerivative(0.), Vector(0., 2.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.GetDerivative(CommonConstantsMath::PI*2.), Vector(0., 2.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.GetDerivative(CommonConstantsMath::PI/4.), Vector(-sqrt(2.), sqrt(2.))) );
//  EXPECT_TRUE(IsEqualVectors(circle.GetDerivative(CommonConstantsMath::PI*9./4.), Vector(-sqrt(2.), sqrt(2.))));
//  EXPECT_TRUE( IsEqualVectors(circle.GetDerivative(CommonConstantsMath::PI), Vector(0., -2.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.GetDerivative(CommonConstantsMath::PI*5./4.), Vector(sqrt(2.), -sqrt(2.))) );
//}
//
//TEST(Circle, Get2Derivative)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  EXPECT_TRUE( IsEqualVectors(circle.Get2Derivative(0.), Vector(-2., 0.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.Get2Derivative(CommonConstantsMath::PI*2.), Vector(-2., 0.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.Get2Derivative(CommonConstantsMath::PI/4.), Vector(-sqrt(2.), -sqrt(2.))) );
//  EXPECT_TRUE(IsEqualVectors(circle.Get2Derivative(CommonConstantsMath::PI*9./4.), Vector(-sqrt(2.), -sqrt(2.))));
//  EXPECT_TRUE( IsEqualVectors(circle.Get2Derivative(CommonConstantsMath::PI), Vector(2., 0.)) );
//  EXPECT_TRUE( IsEqualVectors(circle.Get2Derivative(CommonConstantsMath::PI*5./4.), Vector(sqrt(2.), sqrt(2.))) );
//}
//
//TEST(Circle, GetAsPolyLine)
//{
//  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  GeomPolyline polyline1;
//  GeomPolyline polyline2;
//  //circle.GetAsPolyLine(polyline1, CommonConstantsMath::PI/180. );
//  //circle.GetAsPolyLine(polyline1, CommonConstantsMath::PI/180. );  // специально 2 раза, чтобы проверить, что массив очищается
//  //circle.GetAsPolyLine(polyline2, CommonConstantsMath::PI/60. );
// // EXPECT_EQ(polyline1.GetReferensedPoints().size()-1., 3.*(polyline2.GetReferensedPoints().size()-1.) );  // вычитаю 1 из-за замкнутости (дважды добавляется последняя точка)
//}
//
////TEST(Circle, DistanceToPoint)
////{
////  // Проверка, что для окружности работает частный случай, когда расстояние может быть найдено точно.
////  const Ellipse circle( Point(1., 1.), 2., 2., 0. );
////  Point p( 0., 0. );
////  EXPECT_NEAR( circle.DistanceToPoint(p), 2.-sqrt(2.), CommonConstantsMath::NULL_TOL );
////  p = Point( 1., 1. );
////  EXPECT_NEAR( circle.DistanceToPoint(p), 2., CommonConstantsMath::NULL_TOL );
////  p = Point( 1.-sqrt(2.), 1.-sqrt(2.) );
////  EXPECT_NEAR( circle.DistanceToPoint(p), 0, CommonConstantsMath::NULL_TOL );
////  p = Point( 5., 5. );
////  EXPECT_NEAR( circle.DistanceToPoint(p), sqrt(32.)-2., CommonConstantsMath::NULL_TOL );
////}
//
//TEST(Circle, Translation)
//{
//  Ellipse circle( Point(1., 1.), 2., 2., 0. );
//  circle.Translate( 2., -1. );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(5., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(5., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(3.+sqrt(2.), sqrt(2.))) );
//  EXPECT_TRUE(IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9. / 4.), Point(3. + sqrt(2.), sqrt(2.))));
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(1., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.-sqrt(2.), -sqrt(2.))) );
//}
//
//TEST(Circle, Rotation)
//{
//
//  Ellipse circle(Point(1., 1.), 2., 2., 0.);
//  circle.Rotate( CommonConstantsMath::PI/4. );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(0.), Point(1.41421356237309, 2.828427124746)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*2.), Point(1.41421356237309, 2.82842712474618)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI/4.), Point(0., 3.4142135623730)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*9./4.), Point(0., 3.4142135623730)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI), Point(-1.41421356237309, 0.)) );
//  EXPECT_TRUE( IsEqualPoints(circle.GetPoint(CommonConstantsMath::PI*5./4.), Point(0., -0.58578643762690)) );
//}
//
//TEST(Circle, Scaling)
//{
//  Ellipse circle(Point(1., 1.), 2., 2., 0.);
//  circle.Scale( 2., 0.5 );
//  auto diam = DistancePointPoint( circle.GetPoint(0.), circle.GetPoint(CommonConstantsMath::PI) );
//  EXPECT_NEAR( diam, 8., CommonConstantsMath::NULL_TOL );
//  diam = DistancePointPoint( circle.GetPoint(CommonConstantsMath::PI/2.),
//                             circle.GetPoint(CommonConstantsMath::PI*3./2.) );
//  EXPECT_NEAR( diam, 2., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( circle.GetMajorRadius(), 4., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( circle.GetMinorRadius(), 1., CommonConstantsMath::NULL_TOL );
//}
//
//TEST(Ellipse, Construct3PointsOnLine)
//{
//  std::vector<Point> PointsOnLine { Point(0.0, 0.0), Point(1.0, 1.0), Point(2.0, 2.0) };
//  const Ellipse ellipse( PointsOnLine );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, Construct3SamePoints)
//{
//  Point samePoint( 0.0, 0.0 );
//  std::vector<Point> samePoints { samePoint, samePoint, samePoint };
//  const Ellipse ellipse( samePoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, Construct3Points)
//{
//  std::vector<Point> points { Point(0., 0.), Point(1.2, 2.5), Point(0.5, 1.7) };
//  const Ellipse ellipse( points );
//  EXPECT_TRUE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, Construct5SamePoints)
//{
//  Point samePoint( 0.0, 0.0 );
//  std::vector<Point> samePoints { samePoint, samePoint, samePoint, samePoint, samePoint };
//  const Ellipse ellipse( samePoints );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, Construct5PointsOnLine)
//{
//  std::vector<Point> points { Point(0.0, 0.0), Point(1.0, 1.0), Point(4.0, 4.0), Point(3.0, 3.0), Point(1.1, 2.4) };
//  const Ellipse ellipse( points );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, Construct5Points)
//{
//  std::vector<Point> points { Point(0., 0.), Point(1.2, 2.5), Point(0.5, 1.7), Point(1.2, 2.5), Point(1.2, 2.5) };
//  const Ellipse ellipse( points );
//  GeomPolyline poly;
//  ellipse.GetAsPolyLine(poly);
//  EXPECT_NEAR(poly.DistanceToPoint(points[2]), 0., 1.e-2);
//  EXPECT_TRUE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, ConstructCRRA)
//{
//  const Ellipse ellipse( Point(1., 1.), 0, 1., CommonConstantsMath::PI/4. );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, ConstructCRRA2)
//{
//  const Ellipse ellipse( Point(1., 1.), 2., 3., 0. );
//  EXPECT_TRUE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, GetPoint)
//{
//  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
//                             Point(2.7071067811865475, 1.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
//                             Point(2.7071067811865475, 1.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(0.5, 3.5)) );
//  EXPECT_TRUE(IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9. / 4.), Point(0.5, 3.5)));
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(1.292893218813452, 0.29289321881345287)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.5, -1.5)) );
//}
//
//TEST(Ellipse, GetPoint2)
//{
//  std::vector<Point> points = { Point(2., 1.), Point(2.7071067811865475, 1.7071067811865475), Point(0.5, 3.5) };
//  const Ellipse ellipse( points );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
//                             Point(2.7071067811865475, 1.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
//                             Point(2.7071067811865475, 1.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(0.5, 3.5)) );
//  EXPECT_TRUE(IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9. / 4.), Point(0.5, 3.5)));
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(1.292893218813452, 0.29289321881345287)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(3.5, -1.5)) );
//}
//
//TEST(Ellipse, GetRange)
//{
//  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  const auto range = ellipse.GetRange();
//  EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( range.GetEnd(), CommonConstantsMath::PI*2., CommonConstantsMath::NULL_TOL );
//}
//
//TEST(Ellipse, GetDerivative)
//{
//  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(0.), Vector(-2.82842712474619, 2.8284271247461903)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(CommonConstantsMath::PI*2.),
//                             Vector(-2.82842712474619, 2.8284271247461903)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(CommonConstantsMath::PI/4.), Vector(-2.5, 1.5)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(CommonConstantsMath::PI*9./4.), Vector(-2.5, 1.5)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(CommonConstantsMath::PI),
//                             Vector(2.82842712474619, -2.8284271247461903)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.GetDerivative(CommonConstantsMath::PI*5./4.), Vector(2.5, -1.5)) );
//}
//
//TEST(Ellipse, Get2Derivative)
//{
//  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(0.), Vector(-0.7071067811865476, -0.7071067811865475)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(CommonConstantsMath::PI*2.),
//                             Vector(-0.7071067811865476, -0.7071067811865475)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(CommonConstantsMath::PI/4.), Vector(1.5, -2.5)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(CommonConstantsMath::PI*9./4.), Vector(1.5, -2.5)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(CommonConstantsMath::PI),
//                             Vector(0.7071067811865479, 0.7071067811865471)) );
//  EXPECT_TRUE( IsEqualVectors(ellipse.Get2Derivative(CommonConstantsMath::PI*5./4.), Vector(-1.5, 2.5)) );
//}
//
//TEST(Ellipse, GetAsPolyLine)
//{
//  const Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  GeomPolyline points1, points2;
//  //ellipse.GetAsPolyLine( points1, 1.e-4 );
//  //ellipse.GetAsPolyLine( points2, CommonConstantsMath::PI/60. );
//  //EXPECT_TRUE( points1.GetReferensedPoints().size() > points2.GetReferensedPoints().size() );
//  //// Примитивная проверка, что расстояние между соседними точками зависит от кривизны участка кривой, к которому они
//  //// относятся.
//  //const  auto dist1 = DistancePointPoint(points1.GetReferensedPoints()[0], points1.GetReferensedPoints()[1]);
//  //const  auto dist2 = DistancePointPoint(points2.GetReferensedPoints()[points2.GetReferensedPoints().size()/4], points2.GetReferensedPoints()[points2.GetReferensedPoints().size()/4 + 1]);
//  //EXPECT_TRUE( 1.1 * dist1 < dist2 );
//}
//
//TEST(Ellipse, DistanceToPoint)
//{
//  //// Проверка, что для окружности работает частный случай, когда расстояние может быть найдено точно.
//  //const Ellipse ellipse( Point(2., 1.), 1., 3., 0. );
//  //Point p( 0., 1. );
//  //EXPECT_NEAR( ellipse.DistanceToPoint(p), 1., 1.e-3 );
//  //p = Point( 2., 1. );
//  //EXPECT_NEAR( ellipse.DistanceToPoint(p), 1., 1.e-3 );
//  //p = Point( 2., 4. );
//  //EXPECT_NEAR( ellipse.DistanceToPoint(p), 0., 1.e-3 );
//  //p = Point( 2., 3.99 );
//  //EXPECT_NEAR( ellipse.DistanceToPoint(p), 0.01, 1.e-3 );
//}
//
//TEST(Ellipse, Translation)
//{
//  Ellipse ellipse( Point(2., 1.), 1., 4., CommonConstantsMath::PI/4. );
//  ellipse.Translate( 2., -1. );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.),
//                             Point(4.7071067811865475, 0.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.),
//                             Point(4.7071067811865475, 0.7071067811865475)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(2.5, 2.5)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.), Point(2.5, 2.5)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI),
//                             Point(3.292893218813452, 0.29289321881345287-1.)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(5.5, -2.5)) );
//}
//
//TEST(Ellipse, Rotate1)
//{
//  Ellipse ellipse( Point(0., 0.), 1., 4., CommonConstantsMath::PI/4. );
//  ellipse.Rotate( -CommonConstantsMath::PI/4. );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.), Point(1., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.), Point(1., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.),
//                             Point(0.7071067811865476, 2.82842712474619)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.),
//                             Point(0.7071067811865476, 2.82842712474619)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(-1., 0.)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.),
//                             Point(-0.7071067811865477, -2.82842712474619)) );
//}
//
//TEST(Ellipse, Rotate2)
//{
//  Ellipse ellipse( Point(3., 2.), 3., 2., 0. );
//  ellipse.Rotate( CommonConstantsMath::PI/2. );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.), Point(-2.0, 6.0)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*2.), Point(-2.0, 6.0)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI/4.), Point(-3.41421356237309, 5.1213203435596)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*9./4.), Point(-3.41421356237309, 5.1213203435596)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), Point(-2.0, 0.0)) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI*5./4.), Point(-0.58578643762690, 0.87867965644035)) );
//}
//
//TEST(Ellipse, Scale)
//{
//  Ellipse ellipse( Point(2., 1.), 1., 4., 0. );
//  ellipse.Scale( 2., 0.5 );
//  auto diam = DistancePointPoint( ellipse.GetPoint(0.), ellipse.GetPoint(CommonConstantsMath::PI) );
//  EXPECT_NEAR( diam, 4., CommonConstantsMath::NULL_TOL );
//  diam = DistancePointPoint( ellipse.GetPoint(CommonConstantsMath::PI/2.),
//                             ellipse.GetPoint(CommonConstantsMath::PI*3./2.) );
//  EXPECT_NEAR( diam, 4., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( ellipse.GetMajorRadius(), 2., CommonConstantsMath::NULL_TOL );
//  EXPECT_NEAR( ellipse.GetMinorRadius(), 2., CommonConstantsMath::NULL_TOL );
//}
//
//TEST(Ellipse, Scale2)
//{
//  Point center(3., 2.), p1(9., 6.), p2(1., 5.);
//  std::vector<Point> ellPnts = {center, p1, p2};
//  Point p3( -3., -2. );
//  Ellipse ellipse( ellPnts );
//  // Проверим, что получили ожидаемое начальное состояние.
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetCenter(), center) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(0.), p1) );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetPoint(CommonConstantsMath::PI), p3) );
//  // Проведем масштабирующее преобразование эллипса и точек, на основе которых мы его породили.
//  const double scaleX = 2., scaleY = 1;
//  center.Scale( scaleX, scaleY );
//  p1.Scale( scaleX, scaleY );
//  p2.Scale( scaleX, scaleY );
//  ellipse.Scale( scaleX, scaleY );
//  // Проверим, что после преобразования сохранилось соответствие между эллипсом и породившими его точками.
//  GeomPolyline poly;
//  ellipse.GetAsPolyLine( poly );
//  EXPECT_TRUE( IsEqualPoints(ellipse.GetCenter(), center) );
////  EXPECT_NEAR( poly.DistanceToPoint(p1), 0., 1.e-3 );
//
//  EXPECT_NEAR(Math::Distance(ellipse.GetPoint(ellipse.GetRange().GetEnd()/2.), ellipse.GetPoint(ellipse.GetRange().GetEnd())), ellipse.GetMajorRadius() * 2.0, 1e-7);
//
//}
//
//TEST(Ellipse, IsValid)
//{
//
//  Ellipse ellipse(Point(2., 1.), 1., 4., 0.);
//  EXPECT_TRUE( ellipse.IsValid() );
//  ellipse.Scale( 2., 0.2 );
//  EXPECT_TRUE( ellipse.IsValid() );
//  ellipse.Scale( 1., 0 );
//  EXPECT_FALSE( ellipse.IsValid() );
//}
//
//TEST(Ellipse, IsValid2)
//{
//  Ellipse ellipse(Point(2., 1.), 1., 0., 0.);
//  EXPECT_FALSE( ellipse.IsValid() );
//  Ellipse ellipse2(Point(2., 1.), 1., 1., 0.);
//  EXPECT_TRUE( ellipse2.IsValid() );
//}
//
//
////// Правило экстраполяции: один в один взято с правила экстраполяции полилинии:
////// приведение к граничным точкам всего того, что лежит за пределами области определения
////TEST( Ellipse, Extrapole )
////{
////  Ellipse ellipse( Point(), 1., 1., 0. );
////  ASSERT_TRUE( ellipse.IsValid() );
////  EXPECT_NEAR( Math::Distance( ellipse.GetPoint( ellipse.GetRange().GetStart() - 1.7 ), ellipse.GetPoint( ellipse.GetRange().GetStart() ) ), 0.0, 1e-7 );
////  EXPECT_NEAR( Math::Distance( ellipse.GetPoint( ellipse.GetRange().GetEnd() ), ellipse.GetPoint( ellipse.GetRange().GetEnd() + 2.4 ) ), 0.0, 1e-7 );
////}
//
//// Грубое вычисление габарита
//static void Gabarit( Curve& crv, double& rx, double& ry ) {
//  GeomPolyline polyRep;
//  crv.GetAsPolyLine( polyRep );
//  if ( polyRep.IsValid() ) {
//    auto approxPts = polyRep.GetReferensedPoints();
//    double gab[] = { 100.0, -100.0, 100.0, -100.0 };
//    for ( size_t i = 0; i < approxPts.size(); i++ ) {
//      const Point& curPt = approxPts[i];
//      if ( !curPt.IsValid() )
//        continue;
//      if ( gab[0] > curPt.GetX() )
//        gab[0] = curPt.GetX();
//      if ( gab[1] < curPt.GetX() )
//        gab[1] = curPt.GetX();
//      if ( gab[2] > curPt.GetY() )
//        gab[2] = curPt.GetY();
//      if ( gab[3] < curPt.GetY() )
//        gab[3] = curPt.GetY();
//    }
//    rx = gab[1] - gab[0];
//    ry = gab[3] - gab[2];
//  }
//}
//
//
//TEST( Ellipse, ScaleRotated )
//{
//  Ellipse ellipse( Point(), 2., 1., Math::CommonConstantsMath::PI / 6.0 );
//  ASSERT_TRUE( ellipse.IsValid() );
//  double gabarits[4];
//  Gabarit( ellipse, gabarits[0], gabarits[1] );
//  ellipse.Scale( 2.0, 1.0 );
//  ASSERT_TRUE( ellipse.IsValid() );
//  Gabarit( ellipse, gabarits[2], gabarits[3] );
//  EXPECT_NEAR( gabarits[0] * 2, gabarits[2], 0.5 );
//  EXPECT_NEAR( gabarits[1], gabarits[3], 0.5 );
//}
//
//
//TEST( Ellipse, ScaleUnscale )
//{
//  const double r1 = 2.0, r2 = 1.0, alpha = Math::CommonConstantsMath::PI / 6.0;
//  Ellipse ellipse( Point(), r1, r2, alpha );
//  ASSERT_TRUE( ellipse.IsValid() );
//  const double scaleF = 2.0, rScaleF = 1.0 / scaleF;
//  ellipse.Scale( scaleF, 1.0 );
//  ellipse.Scale( rScaleF, 1.0 );
//
//  const double tol = 1e-5;
//
// //  EXPECT_NEAR( ellipse.GetMajorRadius(), r1, tol );
// // EXPECT_NEAR( ellipse.GetMinorRadius(), r2, tol );
// // EXPECT_NEAR( ellipse.GetAlphaMajorAxis(), alpha, tol );
//}