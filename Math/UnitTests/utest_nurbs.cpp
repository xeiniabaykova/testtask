//
// Created by alakhverdyants on 22.05.17.
//


TEST(NURBSTest, CheckPerimeter)
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
