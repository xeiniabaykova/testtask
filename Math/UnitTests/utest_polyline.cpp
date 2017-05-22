//
// Created by alakhverdyants on 22.05.17.
//


TEST(PolylineTest, CheckPerimeter)
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

