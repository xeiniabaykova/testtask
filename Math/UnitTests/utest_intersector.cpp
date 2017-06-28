#include <gtest/gtest.h>
#include "../Point.h"
#include "../Line.h"
#include "../Ellipse.h"
#include "GeomPolyline.h"
#include "../Intersector.h"
#include "../NurbsCurve.h"
#include <memory>

// в простейшем случае точка пересечения находится верно
TEST(Line, Intersect1Point)
{
	// пересечение двух прямых
	Math::Line line1( Math::Point(1., 1.), Math::Point(3., 3.) );
	Math::Line line2( Math::Point(3., 1.), Math::Point(1., 3.) );
	std::vector<Math::Point> points = Math::IntersectLines(line1, line2);
	EXPECT_FALSE( points.empty() );
	EXPECT_TRUE( Math::IsEqual(line1.GetPoint(points[0].GetX()), line2.GetPoint(points[0].GetY())) );
	// начало одной прямой - конец другой прямой
	Math::Line line3( Math::Point(3.,1.), Math::Point(5., 1.) );
	points = Math::IntersectLines(line3, line2);
	EXPECT_FALSE(points.empty());
	EXPECT_TRUE( Math::IsEqual(line3.GetPoint(points[0].GetX()), line2.GetPoint(points[0].GetY())));
}

// если пересечения нет, возвращаем пустоту
TEST(Line, NoIntersect)
{
	Math::Line line1(Math::Point(1., 3.), Math::Point(3., 3.));
	Math::Line line2(Math::Point(1., 1.), Math::Point(3., 1.));
	std::vector<Math::Point> points = Math::IntersectLines(line1, line2);
	EXPECT_TRUE(points.empty());
}

TEST(GeomPolyline, Intersect1Point)
{
	std::vector<Math::Point> refPointsPolyline1;
	refPointsPolyline1.push_back( Math::Point(0., 0.) );
	refPointsPolyline1.push_back( Math::Point(1., 1.) );
	refPointsPolyline1.push_back( Math::Point(2., 1.) );
	refPointsPolyline1.push_back( Math::Point(2., 2.) );
	refPointsPolyline1.push_back( Math::Point(2., 3.) );
	refPointsPolyline1.push_back( Math::Point(3., 3.) );
	Math::GeomPolyline polyline1( refPointsPolyline1 );

	std::vector<Math::Point> refPointsPolyline2;
	refPointsPolyline2.push_back( Math::Point(0., 3.) );
	refPointsPolyline2.push_back( Math::Point(0., 2.) );
	refPointsPolyline2.push_back( Math::Point(2., 2.) );
	refPointsPolyline2.push_back( Math::Point(3., 1.) );
	refPointsPolyline2.push_back( Math::Point(3., 2.) );
	refPointsPolyline2.push_back( Math::Point(5., 2.) );
	Math::GeomPolyline polyline2( refPointsPolyline2 );

	std::vector<Math::Point> points = Math::IntersectPolylinePolyline( polyline1, polyline2 );
	EXPECT_FALSE( points.empty() );
	EXPECT_TRUE( Math::IsEqual(polyline1.GetPoint(points[0].GetX()), polyline2.GetPoint(points[0].GetY())) );

	refPointsPolyline1.push_back( Math::Point(5., 0.) );

	polyline1.Init( refPointsPolyline1 );
	points = Math::IntersectPolylinePolyline(polyline1, polyline2);
	EXPECT_FALSE( points.empty() );
  EXPECT_TRUE(Math::IsEqual(polyline1.GetPoint(points[0].GetX()), polyline2.GetPoint(points[0].GetY())));
  EXPECT_TRUE(Math::IsEqual(polyline1.GetPoint(points[1].GetX()), polyline2.GetPoint(points[1].GetY())));

}
TEST(Ellipse, 2Intersect)
{
  Math::Ellipse ellipse1( Math::Point(0., 0.), 2., 1., 0. );
  Math::Ellipse ellipse2(Math::Point(0.5, 0.), 2., 1., 0.);
  Math::Line line(Math::Point(-2., 1.), Math::Point(0., 0.));
  std::vector<Math::Point> points = Math::IntersectGeneralCase(ellipse1, ellipse2);
  EXPECT_FALSE(points.empty());
  EXPECT_TRUE(Math::IsEqual(ellipse1.GetPoint(points[0].GetX()), ellipse2.GetPoint(points[0].GetY())));
  EXPECT_TRUE(Math::IsEqual(ellipse1.GetPoint(points[1].GetX()), ellipse2.GetPoint(points[1].GetY())));

}
using namespace Math;

TEST(Nurbs, 1Intersect)
{
  std::vector<Point> poles{ Point(-14., -5.), Point(-7.2, 7.), Point(-2.4, 7.), Point(3., 2.1),
    Point(9., -4.8), Point(15., -2.), Point(19., 8.), Point(13.4, 9.3) };
  std::vector<Point> poles2{ Point(-20, -10.), Point(1., 7.), Point(3, 7.), Point(1., 2.1),
    Point(4., 8), Point(2., 2.), Point(6., 6.), Point(10, 4) };
  std::vector<double> weights{ 1, 1, 1, 1, 1, 1, 1, 1 };
  std::vector<double> nodes{ 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5 };
  std::vector<double> closedNodes{ -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  NurbsCurve curve1( poles, weights, false, 3 );
  NurbsCurve curve2( poles2, weights, false, 3 );

  std::vector<Math::Point> points = Math::IntersectGeneralCase( curve1, curve2 );
  EXPECT_FALSE( points.empty() );
  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve2.GetPoint(points[0].GetY())) );

}

TEST(Nurbs, 2Intersect)
{
  std::vector<Point> poles{ Point(-14., -5.), Point(-7.2, 7.), Point(-2.4, 7.), Point(3., 2.1),
    Point(9., -4.8), Point(15., -2.), Point(19., 8.), Point(13.4, 9.3) };
  std::vector<Point> poles2{ Point(-20, -10.), Point(1., 7.), Point(3, 7.), Point(1., 2.1),
    Point(4., 8), Point(2., 2.), Point(6., 6.), Point(10, 4) };
  std::vector<double> weights{ 1, 2, 1, 2, 1, 4, 1, 6 };
  std::vector<double> nodes{ 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5 };
  std::vector<double> closedNodes{ -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  NurbsCurve curve1(poles, weights, false, 3);
  NurbsCurve curve2(poles2, weights, false, 3);

  std::vector<Math::Point> points = Math::IntersectGeneralCase(curve1, curve2);
  EXPECT_FALSE(points.empty());
  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve2.GetPoint(points[0].GetY())) );
  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[2].GetX()), curve2.GetPoint(points[2].GetY())) );
  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[1].GetX()), curve2.GetPoint(points[1].GetY())) );

}

TEST(Nurbs, 3Intersect)
{
  std::vector<Point> poles{ 
    Point( 0.185, 8.7904 ), Point( 0.5131, 7.908 ), Point( 1.238, 7.350 ), Point( 1.630, 7.223), Point( 2.103,7.135), Point( 2.463, 7.1939),
    Point( 2.614, 7.4289), Point( 2.6141, 7.722), Point( 2.468, 8.163), Point( 2.307, 8.85259),  Point( 2.195, 8.7218), Point( 2.0231, 8.976),Point(1.845, 9.1429), 
    Point(1.598, 9.260), Point(1.260, 9.36), Point( 0.5507, 9.3682),Point(0.235, 9.2507)};

  std::vector<Point> poles2{ Point( 1.598, 9.65 ), Point( 1.367, 8.584 ), Point(1.636, 8.075 ), Point(2.635, 8.839),
    Point(1.786, 9.857) };

  std::vector<Point> poles3{ 
    Point( 0.038, 6.909 ),  Point( 1.012, 8.633 ), 
    Point( 1.147, 8.300 ), Point( 0.9645, 7.1057 ), Point( 1.464, 8.026 ), Point( 1.437, 6.518 ),  Point (1.888, 7.879 ) };

  std::vector<Point> poles4{ Point(10.598, 5.65), Point(10.367, 8.584), Point(13.636, 8.075), Point(11.635, 8.839),
    Point(11.786, 9.857) };


  std::vector<double> weights{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1, 1, 1, 1, 1, 1 };
  NurbsCurve curve1( poles, weights, true, 6 );
  NurbsCurve curve2( poles2, weights, false, 4 );


  std::vector<Math::Point> points = Math::IntersectGeneralCase(curve1, curve2);
  EXPECT_FALSE(points.empty());
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve2.GetPoint(points[0].GetY())));
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[1].GetX()), curve2.GetPoint(points[1].GetY())));

  NurbsCurve curve3(poles3, weights, false, 3);

  points = Math::IntersectGeneralCase(curve1, curve3);
  EXPECT_FALSE(points.empty());
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve3.GetPoint(points[0].GetY())));
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[1].GetX()), curve3.GetPoint(points[1].GetY())));
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[2].GetX()), curve3.GetPoint(points[2].GetY())));
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[3].GetX()), curve3.GetPoint(points[3].GetY())));
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[4].GetX()), curve3.GetPoint(points[4].GetY())));

  /// случай, когда пересечения кривых нет
  NurbsCurve curve4(poles4, weights, false, 3);
  points = Math::IntersectGeneralCase(curve1, curve4);
  EXPECT_TRUE(points.empty());


}