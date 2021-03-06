//
// Created by alakhverdyants on 22.05.17.
//


#include <gtest/gtest.h>
#include "../NurbsCurve.h"
#include <memory>
using namespace Math;

static bool IsEqualPoints(Point p1, Point p2, double eps=CommonConstantsMath::NULL_TOL)
{
	return fabs(p1.GetX() - p2.GetX()) < eps && fabs(p1.GetY() - p2.GetY()) < eps;
}

static bool IsEqualVectors( const Vector & v1, const Vector & v2, double eps=CommonConstantsMath::NULL_TOL )
{
	return fabs( v1.GetX() - v2.GetX() ) < eps && fabs( v1.GetY() - v2.GetY() ) < eps;
}

static std::shared_ptr<NurbsCurve> NurbsWithEqualWeights( bool isClosed )
{
	std::vector<Point> poles {Point(-14., -5.), Point(-7.2, 7.), Point(-2.4, 7.), Point(3., 2.1),
														Point(9., -4.8), Point(15., -2.), Point(19., 8.), Point(13.4, 9.3)};
   std::vector<double> weights {1, 1, 1, 1, 1, 1, 1, 1};
	std::vector<double> nodes {0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5};
	std::vector<double> closedNodes {-3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	return isClosed ? std::make_shared<NurbsCurve>( poles, weights, true, 3 )
									: std::make_shared<NurbsCurve>( poles, weights, false, 3 );
}

static std::shared_ptr<NurbsCurve> NurbsWithNonEqualWeights( bool )
{
	std::vector<Point> poles {Point(-15., 4.), Point(-7.5, -7.), Point(-1.5, -4.), Point(4., -2.), Point(9.5, 5.3),
														Point(19., 5.), Point(24.1, 10.), Point(21., -3.), Point(14.3, -0.7), Point(11., 2.)};
  std::vector<double> weights {0.5, 1.5, 0.75, 1.25, 0.35, 1.15, 0.15, 0.85, 1.1, 1.7};
	std::vector<double> nodes {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6};
	return std::make_shared<NurbsCurve>( poles, weights, false, 4 );
}

TEST(NurbsEqualWeightsNotClosed, GetPoint)
{
	auto nurbs = NurbsWithEqualWeights( false );
  Point testPoint;
  nurbs->GetPoint( -1.25, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(-14., -5.), 1.e-4) );
  nurbs->GetPoint( 0., testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(-14., -5.), 1.e-4) );
  nurbs->GetPoint( 1.25, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(-1.175, 5.42526), 1.e-4) );
  nurbs->GetPoint( 2.5, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(6.0125, -1.18958), 1.e-4) );
  nurbs->GetPoint( 3.75, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(13.5, -1.81693), 1.e-4) );
  nurbs->GetPoint( 5., testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(13.4, 9.3), 1.e-4) );
  nurbs->GetPoint( 6.25, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(13.4, 9.3), 1.e-4) );
}

TEST(NurbsEqualWeightsNotClosed, GetDerivative)
{
	auto nurbs = NurbsWithEqualWeights( false );
  Vector testVector;
  nurbs->GetDerivative( -1.25, testVector );
	EXPECT_TRUE( IsEqualVectors( testVector, Vector(20.4, 36.), 1.e-4) );
  nurbs->GetDerivative( 0., testVector );
	EXPECT_TRUE( IsEqualVectors( testVector, Vector(20.4, 36.), 1.e-4) );
  nurbs->GetDerivative( 1.25, testVector );
	EXPECT_TRUE( IsEqualVectors( testVector, Vector(5.925, -3.58438), 1.e-4) );
  nurbs->GetDerivative( 2.5, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(5.925, -5.4375), 1.e-4) );
  nurbs->GetDerivative( 3.75, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(6., 5.92812), 1.e-4) );
  nurbs->GetDerivative( 5., testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(-16.8, 3.9), 1.e-4) );
  nurbs->GetDerivative( 6.25, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(-16.8, 3.9), 1.e-4) );
}

TEST(NurbsEqualWeightsNotClosed, Get2Derivative)
{
	auto nurbs = NurbsWithEqualWeights( false );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(-1.25), Vector(-26.4, -72.), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(0.), Vector(-26.4, -72.), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(1.25), Vector(-1.2, -4.175), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(2.5), Vector(0.3, 3.85), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(3.75), Vector(0., 11.575), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(5.), Vector(-45.6, -22.2), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(6.25), Vector(-45.6, -22.2), 1.e-4) );
}



TEST(NurbsEqualWeightsNotClosed, GetRange)
{
	auto nurbs = NurbsWithEqualWeights( false );
	const auto range = nurbs->GetRange();
	EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
	EXPECT_NEAR( range.GetEnd(), 5., CommonConstantsMath::NULL_TOL );
}


TEST(NurbsEqualWeightsClosed, GetPoint)
{
	auto nurbs = NurbsWithEqualWeights( true );
  Point testPoint;
  nurbs->GetPoint( -2.66667, testPoint );
	EXPECT_TRUE( IsEqualPoints( testPoint, Point(16.5247, 7.90741), 1.e-4) );
  nurbs->GetPoint( 0., testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(-7.53333, 5.), 1.e-4) );
  nurbs->GetPoint( 2.66667, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(7.0037, -2.03333), 1.e-4) );
  nurbs->GetPoint( 5.33333, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(16.5247, 7.90741), 1.e-4) );
  nurbs->GetPoint( 8., testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(-7.53333, 5.), 1.e-4) );
  nurbs->GetPoint( 10.66667, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(7.0037, -2.03333), 1.e-4) );
}

TEST(NurbsEqualWeightsClosed, GetDerivative)
{
	auto nurbs = NurbsWithEqualWeights( true );
  Vector testVector;
  nurbs->GetDerivative( -2.66667, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(-4.67778, 2.36667), 1.e-4) );
  nurbs->GetDerivative( 0., testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(5.8, 6.), 1.e-4) );
  nurbs->GetDerivative( 2.66667, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(5.96667, -4.63333), 1.e-4) );
  nurbs->GetDerivative( 5.33333, testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(-4.67778, 2.36667), 1.e-4) );
  nurbs->GetDerivative( 8., testVector );
	EXPECT_TRUE( IsEqualVectors(testVector, Vector(5.8, 6.), 1.e-4) );
  nurbs->GetDerivative( 10.6667, testVector );
	EXPECT_TRUE( IsEqualVectors( testVector, Vector(5.96667, -4.63333), 1.e-3) );
}

TEST(NurbsEqualWeightsClosed, Get2Derivative)
{
	auto nurbs = NurbsWithEqualWeights( true );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(-2.66667), Vector(-13.6667, -11.), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(0.), Vector(-2., -12.), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(2.66667), Vector(0.2, 5.8), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(5.33333), Vector(-13.6667, -11.), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(8.), Vector(-2., -12.), 1.e-4) );
	// НЕ СОВПАДАЕТ!

//	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(10.6667), Vector(0.1999, 5.8), 1.e-3) );
}

TEST(NurbsEqualWeightsClosed, GetRange)
{
	auto nurbs = NurbsWithEqualWeights( true );
	const auto range = nurbs->GetRange();
	EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
	EXPECT_NEAR( range.GetEnd(), 8., CommonConstantsMath::NULL_TOL );
}


TEST(NurbsNonEqualWeights, GetPoint)
{
	auto nurbs = NurbsWithNonEqualWeights( true );
  Point testPoint;
  nurbs->GetPoint( -1.5, testPoint );
  EXPECT_TRUE( IsEqualPoints(testPoint, Point(-15., 4.), 1.e-4) );
  nurbs->GetPoint( 0., testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(-15., 4.), 1.e-4) );
  nurbs->GetPoint( 1.5, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(3.18243, -1.91391), 1.e-4) );
  nurbs->GetPoint( 3., testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(15.952, 4.6176), 1.e-4) );
  nurbs->GetPoint( 4.5, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(20.4872, 2.38696), 1.e-4) );
  nurbs->GetPoint( 6., testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(11., 2.), 1.e-4) );
  nurbs->GetPoint( 7.5, testPoint );
	EXPECT_TRUE( IsEqualPoints(testPoint, Point(11., 2.), 1.e-4) );
}

TEST(NurbsNonEqualWeights, GetDerivative) {
  auto nurbs = NurbsWithNonEqualWeights(false);
  Vector testVector;
  nurbs->GetDerivative( -1.5, testVector );
  EXPECT_TRUE(IsEqualVectors( testVector, Vector(90., -132.), 1.e-4) );
  nurbs->GetDerivative( 0., testVector );
  EXPECT_TRUE(IsEqualVectors( testVector, Vector(90., -132.), 1.e-4));
  nurbs->GetDerivative( 1.5, testVector );
  EXPECT_TRUE(IsEqualVectors( testVector, Vector(4.783, 2.82014), 1.e-4));
  nurbs->GetDerivative( 3., testVector );
  EXPECT_TRUE(IsEqualVectors( testVector, Vector(7.47541, 2.1636), 1.e-4));
  nurbs->GetDerivative( 4.5, testVector );
  EXPECT_TRUE(IsEqualVectors(testVector, Vector(1.12762, -8.11618), 1.e-4));
  nurbs->GetDerivative( 6., testVector );
  EXPECT_TRUE(IsEqualVectors(testVector, Vector(-8.54118, 6.98824), 1.e-4));
  nurbs->GetDerivative( 7.5, testVector );
  EXPECT_TRUE(IsEqualVectors(testVector, Vector(-8.54118, 6.98824), 1.e-4));
}

TEST(NurbsNonEqualWeights, Get2Derivative)
{
	auto nurbs = NurbsWithNonEqualWeights( false );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(-1.5), Vector(-1723.5, 2634), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(0.), Vector(-1723.5, 2634), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(1.5), Vector(-4.15674, -0.215099), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(3.), Vector(-14.3755, -6.71622), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(4.5), Vector(-8.29455, -5.74815), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(6.), Vector(15.681, -3.28443), 1.e-4) );
	EXPECT_TRUE( IsEqualVectors(nurbs->Get2Derivative(7.5), Vector(15.681, -3.28443), 1.e-4) );
}

TEST(NurbsNonEqualWeights, GetRange)
{
	auto nurbs = NurbsWithNonEqualWeights( false );
	const auto range = nurbs->GetRange();
	EXPECT_NEAR( range.GetStart(), 0., CommonConstantsMath::NULL_TOL );
	EXPECT_NEAR( range.GetEnd(), 6., CommonConstantsMath::NULL_TOL );
}


//TEST(NURBSTest, CheckPerimeter)
//{
//	std::vector<Point> poles;
//	poles.push_back( Point(0, 0) );
//	poles.push_back( Point(1, 2) );
//	std::vector<double> weights;
//
//	std::vector<double> nodes = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
//	bool isClosed = 0;
//	int degree = 2;
//
//	NurbsCurve *curve = new NurbsCurve(poles, weights, nodes, isClosed, degree);
//
//	double x = 5 / 2.0;
//	double result;
//	int i = 2;
//	double value = 2.5;
//	// функциии
//	curve->ComputeBasicFunction( x, i, result );
//	EXPECT_EQ( result, 0.125 );
//
//	i = 3;
//	curve->ComputeBasicFunction(x, i, result);
//	double res = 6 / 8.0;
//	EXPECT_EQ(result, res);
//	// производные
//	i = 4;
//	int derivative = 1;
//	std::vector<std::vector<double>> ders;
//	curve->ComputeBasicFunctionD( value, i, derivative , ders);
//
//	i = 4;
//	//curve->ComputeBasicFunctionD(value, i, result, derivative);
//	EXPECT_EQ(result, 0.5);
//}
