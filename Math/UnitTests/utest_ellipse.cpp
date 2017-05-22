//
// Created by alakhverdyants on 22.05.17.
//


TEST( EllipseTest, CheckPerimeter )
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


TEST(CircleTest, CheckPerimeter)
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
