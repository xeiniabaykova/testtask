#include "stdafx.h"
#include "gtest/gtest.h"
#include "Point.h"
#include "Ellipse.h"
#include "GeomPolyline.h"
#include "Line.h"

class EllipseTest : public ::testing::Test {
};

class CircleTest : public ::testing::Test {
};

class PolylineTest : public ::testing::Test {
};
class LineTest : public ::testing::Test {
};

TEST_F( EllipseTest, CheckPerimeter )
{
	Point point( 0.0, 0.0 );
	Point point1( 1.0, 1.0 );
	Point point2( 2.0, 2.0 );
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
	EXPECT_EQ(ellipse4->GetPoint(0).GetX(), 1.1999999999999995);
	EXPECT_EQ(ellipse4->GetPoint(0).GetY(), 2.5000000000000000);

	EXPECT_EQ(ellipse4->GetDerivativePoint(0).GetX(), 0.0  );
	EXPECT_EQ(ellipse4->GetDerivativePoint(0).GetY(), 0.36709825739075497);

	EXPECT_EQ(ellipse4->Get2DerivativePoint(0).GetX(), -2.7730849247724092);
	EXPECT_EQ(ellipse4->Get2DerivativePoint(0).GetY(), 0.0);
	delete ellipse4;



}

TEST_F(CircleTest, CheckPerimeter)
{
	Point samePoint(0.0, 0.0);
	std::vector<Point> samePoints;
	samePoints.push_back(samePoint);
	samePoints.push_back(samePoint);
	Ellipse* ellipse2 = new Ellipse(samePoints);
	ASSERT_FALSE(ellipse2->IsValid());
	delete ellipse2;

	std::vector<Point> wrongNumberPoints;
	Ellipse* ellipse3 = new Ellipse(samePoints);
	ASSERT_FALSE(ellipse3->IsValid());
	delete ellipse3;

}

TEST_F(PolylineTest, CheckPerimeter)
{
	Point samePoint(0.0, 0.0);
	std::vector<Point> samePoints;
	samePoints.push_back(samePoint);
	samePoints.push_back(samePoint);
	samePoints.push_back(samePoint);
	GeomPolyline* polyline = new GeomPolyline(samePoints);
	ASSERT_FALSE( polyline->IsValid() );
	delete polyline;

	std::vector<Point> empty;
	GeomPolyline* polyline1 = new GeomPolyline(empty);
	ASSERT_FALSE(polyline1->IsValid());
	delete polyline1;

	std::vector<Point> polylinePoints;

	
	Point point(0.0, 0.0);
	Point point1(1.0, 1.0);
	Point point2(2.0, 2.0);
	polylinePoints.push_back(point);
	polylinePoints.push_back(point1);
	polylinePoints.push_back(point2);
	GeomPolyline* polyline2 = new GeomPolyline( polylinePoints );
	ASSERT_TRUE(polyline2->IsValid());
	EXPECT_EQ(polyline2->GetPoint(1).GetX(), 2.0);
	EXPECT_EQ(polyline2->GetPoint(1).GetY(), 2.0);

	EXPECT_EQ(polyline2->GetDerivativePoint(1).GetX(), 1.0);
	EXPECT_EQ(polyline2->GetDerivativePoint(1).GetY(), 1.0);

	EXPECT_EQ(polyline2->Get2DerivativePoint(1).GetX(), 0.0);
	EXPECT_EQ(polyline2->Get2DerivativePoint(1).GetY(), 0.0);
	delete polyline2;

}

TEST_F(LineTest, CheckPerimeter)
{
	Point samePoint(0.0, 0.0);
	std::vector<Point> samePoints;
	samePoints.push_back(samePoint);
	samePoints.push_back(samePoint);
	Line* line = new Line(samePoints);
	ASSERT_FALSE(line->IsValid());
	delete line;

}


TEST_F(NURBSTest, CheckPerimeter) 
{
	std::vector<Point> poles;
	poles.push_back( Point(0, 0) );
	poles.push_back( Point(1, 2) );
	std::vector<double> weights;

	std::vector<double> nodes = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
	bool isClosed = 0;
	int degree = 2;

	NurbsCurve *curve = new NurbsCurve(poles, weights, nodes, isClosed, degree);

	double x = 5 / 2.0; 
	double result;
	int i = 2;
	double value = 2.5;
	// функциии
	curve->ComputeBasicFunction( x, i, result );
	EXPECT_EQ( result, 0.125 );

	i = 3;
	curve->ComputeBasicFunction(x, i, result);
	double res = 6 / 8.0;
	EXPECT_EQ(result, res);
	// производные
	i = 3;
	int derivative = 1;
	curve->ComputeBasicFunctionD( value, i, result, derivative );
	EXPECT_EQ( result, 0.0 );
	 
	i = 4;
	curve->ComputeBasicFunctionD(value, i, result, derivative);
	EXPECT_EQ(result, 0.5);


}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}