#include <gtest/gtest.h>
#include "../Point.h"
#include "../Line.h"
#include "../Ellipse.h"
#include "GeomPolyline.h"
#include "../Intersector.h"
#include "../NurbsCurve.h"
#include <memory>
#include <algorithm>
#include <ctime>


namespace {
  template<typename T>
  bool compare(std::vector<T>& v1, std::vector<T>& v2)
  {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
  }
}

//в простейшем случае точка пересечения находится верно
TEST(Line, Intersect1Point)
{
  // пересечение двух прямых
  Math::Line line1(Math::Point(1., 1.), Math::Point(3., 3.));
  Math::Line line2(Math::Point(3., 1.), Math::Point(1., 3.));
  Math::Point point;
  bool haveIntersect = Math::IntersectLines(line1, line2, point);
  EXPECT_TRUE(haveIntersect);
  EXPECT_TRUE(Math::IsEqual(Math::Point(2., 2.), point));
  // начало одной прямой - конец другой прямой
  Math::Line line3(Math::Point(3., 1.), Math::Point(5., 1.));
  haveIntersect = Math::IntersectLines(line3, line2, point);
  EXPECT_TRUE(haveIntersect);
  EXPECT_TRUE(Math::IsEqual(Math::Point(3., 1.), point));
  // отрезки лежат на одной прямой, начало одного - конец другого
  Math::Line line4(Math::Point(3., 3.), Math::Point(4., 4.));
  haveIntersect = Math::IntersectLines(line1, line4, point);
  EXPECT_TRUE(haveIntersect);
  EXPECT_TRUE(Math::IsEqual(Math::Point(3., 3.), point));

  // Совпадение начала
  Math::Line line5(Math::Point(3., 3.), Math::Point(2., 2.));
  haveIntersect = Math::IntersectLines(line5, line4, point);
  EXPECT_TRUE(haveIntersect);
  EXPECT_TRUE(Math::IsEqual(Math::Point(3., 3.), point));

}

// если пересечения нет, возвращаем пустоту
TEST(Line, NoIntersect)
{
  Math::Line line1(Math::Point(1., 3.), Math::Point(3., 3.));
  Math::Line line2(Math::Point(1., 1.), Math::Point(3., 1.));
  Math::Point point;
  bool haveIntersect = Math::IntersectLines(line1, line2, point);
  EXPECT_FALSE(haveIntersect);
}

using namespace Math;
TEST(GeomPolyline, Intersect1Point)
{
  std::vector<Math::Point> refPointsPolyline1;
  refPointsPolyline1.push_back(Math::Point(0., 0.));
  refPointsPolyline1.push_back(Math::Point(1., 1.));
  refPointsPolyline1.push_back(Math::Point(2., 1.));
  refPointsPolyline1.push_back(Math::Point(2., 2.));
  refPointsPolyline1.push_back(Math::Point(2., 3.));
  refPointsPolyline1.push_back(Math::Point(3., 3.));
  Math::GeomPolyline polyline1(refPointsPolyline1);

  std::vector<Math::Point> refPointsPolyline2;
  refPointsPolyline2.push_back(Math::Point(0., 3.));
  refPointsPolyline2.push_back(Math::Point(0., 2.));
  refPointsPolyline2.push_back(Math::Point(2., 2.));
  refPointsPolyline2.push_back(Math::Point(3., 1.));
  refPointsPolyline2.push_back(Math::Point(3., 2.));
  refPointsPolyline2.push_back(Math::Point(5., 2.));
  Math::GeomPolyline polyline2(refPointsPolyline2);

  std::vector<Math::Point> refPointsPolyline3 = {
    Point(0.19613111230521227, 8.3006856023506366),
    Point(0.63138097796883397, 7.9480901077375128),
    Point(1.1740999462654487, 7.5563173359451525),
    Point(2.2111767866738314, 7.144955925563174),
    Point(1.7651800107469102, 8.3790401567091095),
    Point(0.91080064481461587, 9.1234084231145935),
    Point(0.27673293927995701, 9.5641527913809998),
    Point(0.55077915099408925, 9.9069539666993158),
    Point(1.1955937667920473, 9.5739471106758085),
    Point(1.8189145620634068, 9.0156709108716946),
    Point(2.2434175174637292, 8.2713026444662106),
    Point(2.7162815690488986, 7.4289911851126353),
    Point(2.6195593766792049, 8.7218413320274237),
    Point(2.0499731327243418, 9.2899118511263481) };
  Math::GeomPolyline polyline3(refPointsPolyline3);

  std::vector<Math::Point> refPointsPolyline4 = {
    Point(0.4110693175711983, 7.5661116552399612),
    Point(0.48092423428264375, 8.9177277179236043),
    Point(0.78183772165502419, 7.3702252693437806),
    Point(1.0558839333691563, 9.1332027424094022),
    Point(1.4373992477162816, 8.0754162585700),
    Point(1.7651800107469102, 8.8295788442703245),
    Point(2.2756582482536269, 8.6924583741429977),
    Point(2.2917786136485763, 9.5935357492654258),
    Point(1.3191832348199892, 9.642507345739471),
    Point(0.9054271896829662, 9.9265426052889332),
    Point(0.16389038151531435, 7.438785504407444) };

  std::vector<Math::Point> refPointsPolyline5 = {
    Point(0.79258463191832351,6.3320274240940257),
    Point(3.8984416980118217,8.9764936336924581),
    Point(1.6093498119290703	,3.1096963761018612),
    Point(6.7786136485760347,8.1635651322233116),
    Point(3.8823213326168728,1.7678746327130266)
  };
  std::vector<Math::Point> refPointsPolyline6 = {
    Point(0.7281031703385276, 7.8011753183153774),
    Point(5.6663084363245568,2.7864838393731639),
    Point(5.6394411606663084,7.9187071498530859),
    Point(7.299838796346050,5.5190989226248774)
  };
  Math::GeomPolyline polyline5(refPointsPolyline5);
  Math::GeomPolyline polyline6(refPointsPolyline6);

  std::vector<Math::Point> points = Math::IntersectPolylinePolyline(polyline5, polyline6);

  EXPECT_TRUE(points.size() == 8);
  EXPECT_TRUE(Math::IsEqual(points[0], Point(6.260966206, 7.020478568)));
  EXPECT_TRUE(Math::IsEqual(points[1], Point(5.99805787, 7.40043394)));
  EXPECT_TRUE(Math::IsEqual(points[2], Point(5.651192472, 5.673955619)));
  EXPECT_TRUE(Math::IsEqual(points[3], Point(5.643966623, 7.054247184)));
  EXPECT_TRUE(Math::IsEqual(points[4], Point(4.760259166, 3.706566604)));
  EXPECT_TRUE(Math::IsEqual(points[5], Point(3.51415061, 4.971975717)));
  EXPECT_TRUE(Math::IsEqual(points[6], Point(2.6703145486, 5.828881676)));
  EXPECT_TRUE(Math::IsEqual(points[7], Point(1.54444196, 6.972192526)));


}
TEST(Ellipse, 2Intersect)
{
  Math::Ellipse ellipse1(Math::Point(0., 0.), 2., 1., 0.);
  Math::Ellipse ellipse2(Math::Point(0., 0.5), 2., 1., 0.);
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
  NurbsCurve curve1(poles, weights, false, 3);
  NurbsCurve curve2(poles2, weights, false, 3);

  // std::vector<Math::Point> points = Math::IntersectGeneralCase( curve1, curve2 );
  /* EXPECT_FALSE( points.empty() );
  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve2.GetPoint(points[0].GetY())) );*/

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
  EXPECT_TRUE(Math::IsEqual(curve1.GetPoint(points[0].GetX()), curve2.GetPoint(points[0].GetY())));
  //  EXPECT_TRUE( Math::IsEqual(curve1.GetPoint(points[1].GetX()), curve2.GetPoint(points[1].GetY())) );

}

TEST(Nurbs, 3Intersect)
{
  std::vector<Point> poles{
    Point(0.185, 8.7904), Point(0.5131, 7.908), Point(1.238, 7.350), Point(1.630, 7.223), Point(2.103,7.135), Point(2.463, 7.1939),
    Point(2.614, 7.4289), Point(2.6141, 7.722), Point(2.468, 8.163), Point(2.307, 8.85259),  Point(2.195, 8.7218), Point(2.0231, 8.976),Point(1.845, 9.1429),
    Point(1.598, 9.260), Point(1.260, 9.36), Point(0.5507, 9.3682),Point(0.235, 9.2507) };

  std::vector<Point> poles2{ Point(1.598, 9.65), Point(1.367, 8.584), Point(1.636, 8.075), Point(2.635, 8.839),
    Point(1.786, 9.857) };

  std::vector<Point> poles3{
    Point(0.038, 6.909),  Point(1.012, 8.633),
    Point(1.147, 8.300), Point(0.9645, 7.1057), Point(1.464, 8.026), Point(1.437, 6.518),  Point(1.888, 7.879) };

  std::vector<Point> poles4{ Point(10.598, 5.65), Point(10.367, 8.584), Point(13.636, 8.075), Point(11.635, 8.839),
    Point(11.786, 9.857) };


  std::vector<double> weights{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1, 1, 1, 1, 1, 1 };
  NurbsCurve curve1(poles, weights, true, 3);
  NurbsCurve curve2(poles2, weights, false, 4);


  std::vector<Math::Point> points = Math::IntersectGeneralCase(curve1, curve2);
  EXPECT_TRUE(points.empty());
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

using namespace Math;
TEST(Nurbs, 15Intersect)
{
  std::vector<Point> poles{
    Point(0.185, 8.7904), Point(0.5131, 7.908), Point(1.238, 7.350), Point(1.630, 7.223), Point(2.103,7.135), Point(2.463, 7.1939),
    Point(2.614, 7.4289), Point(2.6141, 7.722), Point(2.468, 8.163), Point(2.307, 8.85259),  Point(2.195, 8.7218), Point(2.0231, 8.976),Point(1.845, 9.1429),
    Point(1.598, 9.260), Point(1.260, 9.36), Point(0.5507, 9.3682),Point(0.235, 9.2507) };

  std::vector<Point> poles2{ Point(1.598, 9.65), Point(1.367, 8.584), Point(1.636, 8.075), Point(2.635, 8.839),
    Point(1.786, 9.857) };

  std::vector<Point> poles3{
    Point(0.038, 6.909),  Point(1.012, 8.633),
    Point(1.147, 8.300), Point(0.9645, 7.1057), Point(1.464, 8.026), Point(1.437, 6.518),  Point(1.888, 7.879) };

  std::vector<Point> poles4{ Point(10.598, 5.65), Point(10.367, 8.584), Point(13.636, 8.075), Point(11.635, 8.839),
    Point(11.786, 9.857) };

  std::vector<Point> poles5{
    Point(11.185, 5.7904), Point(1.5131, 6.908), Point(2.238, 5.350), Point(4.630, 3.223), Point(7.103,7.135), Point(5.463, 7.1939),
    Point(6.614, 4.4289), Point(3.6141, 2.722), Point(1.468, 4.163), Point(1.307, 5.85259),  Point(2.195, 3.7218), Point(4.0231, 5.976),Point(3.845, 5.1429),
    Point(6.598, 7.260), Point(1.260, 9.36), Point(1.5507, 3.3682),Point(0.235, 9.2507) };

  std::vector<Point> poles6{
    Point(0.22837184309511016, 9.4466209598432922),
    Point(0.26598602901665774,	7.8011753183153774),
    Point(0.32509403546480387,6.8119490695396676),
    Point(0.37882858678130038, 4.5984329089128311),
    Point(0.85706609349811935,2.8452497551420177),
    Point(1.5932294465341215,1.4446620959843293),
    Point(3.280494357872112, 0.77864838393731639),
    Point(5.3600214938205264,1.346718903036239),
    Point(6.5314347125201504,1.385896180215475),
    Point(7.8318108543793663,	1.405484818805093),
    Point(8.584094572810316,	2.22820763956905),
    Point(8.1864588930682434,5.3330068560235064),
    Point(6.7947340139709835,6.6650342801175322),
    Point(4.5862439548629768,	7.3212536728697364),
    Point(1.217087587318646,	8.2223310479921654),
    Point(1.2708221386351424, 9.2213516160626838),
    Point(2.5819451907576574, 9.0450538687561224),
    Point(4.9731327243417516,8.7512242899118515),
    Point(6.6442772702847934,8.2713026444662106),
    Point(8.2079527135948425,7.7228207639569053),
    Point(8.2186996238581411,8.8589618021547505),
    Point(7.4180548092423431,	9.2801175318315376),
    Point(6.1929070392262222,9.4857982370225269),
    Point(3.8823213326168728,9.701273261508326),
    Point(2.248790972595379,9.9755142017629783)
  };

  std::vector<Point> poles7{
    Point(1.0720042987641054,6.1851126346718903),
    Point(0.91080064481461587, 4.4025465230166505),
    Point(1.3782912412681354,2.9236043095004898),
    Point(3.0977968833960237,2.3849167482859941),
    Point(4.1886082751209033,2.0323212536728699),
    Point(6.2305212251477702,1.7286973555337906),
    Point(6.9183234819989252,2.6003917727717925),
    Point(4.2047286405158522,4.5690499510284042),
    Point(2.2702847931219776,5.9990205680705193),
    Point(2.2272971520687803, 7.6346718903036246),
    Point(2.6356797420741538, 8.3692458374143008),
    Point(3.2267598065556156, 8.418217433888346),
    Point(4.0757657173562603, 8.6043095004897161),
    Point(4.4035464803868889, 9.4955925563173373),
    Point(5.0591080064481462,10.073457394711069),
    Point(5.3116603976356798,9.8285994123408429),
    Point(6.0424502955400321, 7.5954946131243881)
  };

  std::vector<Point> poles8{
    Point(0.08328855454056959 ,8.7120470127326151),
    Point(0.54540569586243959,8.0362389813907935),
    Point(0.16389038151531435,5.6953966699314398),
    Point(0.63138097796883397,5.2546523016650344),
    Point(0.14239656098871575,2.7571008814887366),
    Point(2.5711982804943578,4.0401567091087172),
    Point(4.3659322944653409,5.7443682664054849),
    Point(6.0155830198817846,6.9784524975514204),
    Point(8.0897367006985501,7.2037218413320279),
    Point(6.8430951101558302,5.0293829578844269),
    Point(6.0424502955400321,3.8736532810969639),
    Point(4.8710370768404081,3.2859941234084231)
  };
  std::vector<Point> poles9{
    Point(0.91617409994626553, 9.7110675808031353),
    Point(0.58301988178398712,9.1919686581782578),
    Point(0.040300913487372379,8.0460333006856022),
    Point(-0.02418054809242343,6.9882468168462299),
    Point(0.099408919935518536,6.8217433888344763),
    Point(0.50779150994089206,6.6748285994123409),
    Point(0.64750134336378296,5.8325171400587665),
    Point(0.87318645889306823,5.1665034280117537),
    Point(1.2009672219236969,4.9608227228207644),
    Point(1.7974207415368082,4.5690499510284042),
    Point(1.9317571198280494,3.6777668952007838),
    Point(2.4744760881246641,3.1096963761018612),
    Point(3.0924234282643739,3.4231145935357494),
    Point(3.522299838796346,2.9431929480901080),
    Point(3.8984416980118217,3.0215475024485801),
    Point(4.7689414293390655,.6395690499510285),
    Point(5.1450832885545408,2.4045053868756123),
    Point(5.5480924234282645,1.7189030362389814),
    Point(5.8597528210639442,2.4632713026444661),
    Point(6.0854379365932294,3.3153770812928505),
    Point(5.3600214938205264,5.1175318315377085),
    Point(4.5647501343363786,6.1851126346718903),
    Point(3.5276732939279958,6.4887365328109698),
    Point(2.0660934981192907,7.5367286973555343),
    Point(2.1520687802256853,8.5357492654260536),
    Point(3.3342289091886084,8.2027424094025463),
    Point(3.7426114991939818,7.6640548481880515),
    Point(3.9629231595916177,6.6454456415279139),
    Point(4.6131112305212252,6.6748285994123409),
    Point(4.9838796346050511,5.9696376101860924),
    Point(5.3815153143471255, 5.9304603330068559),
    Point(5.8973670069854922,5.3134182174338882),
    Point(6.004836109618485,6.1557296767874634),
    Point(5.1719505642127892,7.3800195886385902),
    Point(5.0859752821063946,8.0950048971596473),
    Point(5.4191295002686726,8.1635651322233116),
    Point(5.6179473401397102,7.7032321253672871),
    Point(6.1337990327780769,7.2428991185112634),
    Point(6.5206878022568517,7.5563173359451525),
    Point(6.8430951101558302,7.5857002938295794),
    Point(6.9183234819989252,7.1155729676787471),
    Point(6.9075765717356266,6.5475024485798237)
  };
  std::vector<Point> poles10{
    Point(1.4373992477162816,	9.5935357492654258),
    Point(1.3191832348199892,	7.7130264446620966),
    Point(1.5663621708758733,	6.9490695396669935),
    Point(1.7651800107469102,	6.3712047012732622),
    Point(2.2058033315421817,	5.2840352595494613),
    Point(2.4744760881246641,	4.8824681684622924),
    Point(2.7807630306286941,	5.3525954946131247),
    Point(2.8774852229983883,	5.9304603330068559),
    Point(3.1998925308973671,	5.8716944172380021),
    Point(3.4148307361633532,	4.5886385896180215),
    Point(3.5975282106394411,	3.805093046033301),
    Point(4.6453519613111229,	3.6385896180215478),
    Point(5.2632993014508331,	3.0215475024485801),
    Point(6.1015583019881783,	2.4632713026444661),
    Point(6.5583019881783988,	3.129285014691479),
    Point(6.2197743148844706,	5.1077375122428998),
    Point(5.6340677055346591,	5.9108716944172386),
    Point(5.1074691026329928,	6.7140058765915773),
    Point(4.4250403009134871,	7.144955925563174),
    Point(3.0870499731327246,	7.5171400587659161),
    Point(2.6088124664159054,	6.8805093046033301),
    Point(2.0069854916711445,	6.9000979431929483),
    Point(1.738312735088662,	7.7619980411361418)
  };

  std::vector<double> weights{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1, 1, 1, 1, 1, 1 };
  std::vector<double> weights2{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  std::vector<double> weigts3{ 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  std::vector<double> weights4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
  NurbsCurve curve1(poles, weights, true, 6);
  NurbsCurve curve2(poles2, weights, false, 4);
  NurbsCurve curve3(poles3, weights, false, 4);
  NurbsCurve curve4(poles4, weights, false, 4);
  NurbsCurve curve5(poles5, weights, false, 4);
  NurbsCurve curve6(poles6, weights2, false, 4);
  NurbsCurve curve7(poles7, weigts3, false, 4);
  NurbsCurve curve8(poles8, weigts3, false, 4);
  NurbsCurve curve9(poles9, weights4, false, 4);
  NurbsCurve curve10(poles10, weights4, false, 4);
  std::vector<Curve*> curves;
  curves.push_back(&curve1);
  curves.push_back(&curve2);

  curves.push_back(&curve3);
  curves.push_back(&curve4);
  curves.push_back(&curve5);
  curves.push_back(&curve6);
  curves.push_back(&curve7);
  curves.push_back(&curve8);
  curves.push_back(&curve9);
  curves.push_back(&curve10);
  clock_t t = clock();

  /* std::vector<Math::Point> points = Math::IntersectGeneralCase(curve1, curve2);
  for ( size_t i=0; i < curves.size(); i++ )
  for ( size_t j = 0; j < curves.size(); j++ )
  {
  if ( i !=j )
  points = Math::IntersectGeneralCase(*curves[i], *curves[j]);
  }
  */

  int time = (clock() - t) / CLOCKS_PER_SEC;
  std::cout << time;
  // std::cin.get();
  //  std::cin.get();
}

TEST(segmenttest, Intersector)
{
  std::vector<Line> segments;
  segments.push_back(Line(Point(1.0, 1.0), Point(2.0, 3.0)));
  segments.push_back(Line(Point(2.0, 1.0), Point(4.0, 3.0)));
  segments.push_back(Line(Point(2.0, 6.0), Point(4.0, 1.0)));
  segments.push_back(Line(Point(1.0, 4.0), Point(4.0, 5.0)));

  //std::vector<Point> points = segmentsIntersections( segments );

}
//using namespace Math;
//TEST(SegmentsIntersectionsTest, lineCurveFirst)
//{
//  
//  // простейший случай: разные начала и концы ( не совпадают ни по х, ни по у), пересечения нет
//  std::vector<Math::Point> refPointsPolyline1;
//  std::vector<Math::Point> refPointsPolyline2;
//  std::vector<Line> segments1;
//  std::vector<Line> segments2;
//  refPointsPolyline1.push_back( Point(2.0, 2.0) );
//  refPointsPolyline1.push_back( Point(4.0, 5.0) );
//  refPointsPolyline2.push_back( Point(6.0, 2.0) );
//  refPointsPolyline2.push_back( Point(8.0, 5.0) );
//  Math::GeomPolyline polyline1(refPointsPolyline1);
//  Math::GeomPolyline polyline2(refPointsPolyline2);
//  std::vector<std::pair<double, double>> refParams;
//  std::vector<Point> points = SegmentsIntersections( &polyline1, &polyline2, refParams );
//  std::vector<Point> testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_TRUE( points.empty());
//  EXPECT_TRUE(compare(points, testpoints));
//  // простейший случай: разные начала и концы ( не совпадают ни по х, ни по у), пересечение есть
//  refPointsPolyline1.clear();
//  refPointsPolyline2.clear();
//
//  refPointsPolyline1.push_back(Point(2.0, 2.0));
//  refPointsPolyline1.push_back(Point(4.0, 5.0));
//  refPointsPolyline2.push_back(Point(1.0, 5.0));
//  refPointsPolyline2.push_back(Point(4.0, 2.0));
//  polyline1.Init( refPointsPolyline1 );
//  polyline2.Init( refPointsPolyline2 );
//  refParams.clear();
//  points = SegmentsIntersections( &polyline1, &polyline2, refParams);
//  testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_FALSE(points.empty());
//  EXPECT_TRUE(compare(points, testpoints));
//
//  // концы отрезков совпадают
//  refPointsPolyline1.clear();
//  refPointsPolyline2.clear();
//
//  refPointsPolyline1.push_back(Point(1.0, 3.0));
//  refPointsPolyline1.push_back(Point(3.0, 1.0));
//  refPointsPolyline2.push_back(Point(1.0, 1.0));
//  refPointsPolyline2.push_back(Point(3.0, 1.0));
//  polyline1.Init(refPointsPolyline1);
//  polyline2.Init(refPointsPolyline2);
//  refParams.clear();
//  points = SegmentsIntersections(&polyline1, &polyline2, refParams );
//  testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_FALSE(points.empty());
//  EXPECT_TRUE(compare(points, testpoints));
//
//
//  // пересечение вертикального и горизонтального отрезков
//
//  refPointsPolyline1.clear();
//  refPointsPolyline2.clear();
//
//  refPointsPolyline1.push_back(Point(2.0, 1.0));
//  refPointsPolyline1.push_back(Point(2.0, 3.0));
//  refPointsPolyline2.push_back(Point(1.0, 1.0));
//  refPointsPolyline2.push_back(Point(3.0, 1.0));
//  polyline1.Init(refPointsPolyline1);
//  polyline2.Init(refPointsPolyline2);
//  refParams.clear();
//  points = SegmentsIntersections(&polyline1, &polyline2, refParams);
//  testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_FALSE(points.empty());
//  EXPECT_TRUE(compare(points, testpoints));
//
//  refPointsPolyline1.clear();
//  refPointsPolyline2.clear();
//
//
//  refPointsPolyline1.push_back(Math::Point(0., 0.));
//  refPointsPolyline1.push_back(Math::Point(1., 1.));
//  refPointsPolyline1.push_back(Math::Point(2., 1.));
//  refPointsPolyline1.push_back(Math::Point(2., 2.));
//  refPointsPolyline1.push_back(Math::Point(2., 3.));
//  refPointsPolyline1.push_back(Math::Point(3., 3.));
//  polyline1.Init(refPointsPolyline1);
//
//
//  refPointsPolyline2.push_back(Math::Point(0., 3.));
//  refPointsPolyline2.push_back(Math::Point(0., 2.));
//  refPointsPolyline2.push_back(Math::Point(2., 2.));
//  refPointsPolyline2.push_back(Math::Point(3., 1.));
//  refPointsPolyline2.push_back(Math::Point(3., 2.));
//  refPointsPolyline2.push_back(Math::Point(5., 2.));
//  polyline2.Init(refPointsPolyline2);
//  refParams.clear();
//  points = SegmentsIntersections( &polyline1, &polyline2, refParams );
//  testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_TRUE(compare(points, testpoints));
//
//  refPointsPolyline1.clear();
//  refPointsPolyline2.clear();
//
//  refPointsPolyline1.push_back(Point(-0.051047823750671684, 8.8687561214495592));
//  refPointsPolyline1.push_back(Point(0.2122514777001612, 8.7610186092066602));
//  refPointsPolyline1.push_back(Point(0.45943041375604515, 9.5151811949069547));
//  refPointsPolyline1.push_back(Point(0.76034390112842565, 8.3986287952987269));
//  refPointsPolyline1.push_back(Point(0.94841483073616339, 9.3192948090107741));
//  refPointsPolyline1.push_back(Point(1.1794734013970984, 8.2125367286973567));
//  refPointsPolyline1.push_back(Point(1.5341214400859753, 8.9862879529872686));
//  refPointsPolyline1.push_back(Point(1.8081676518001075, 7.9578844270323215));
//  refPointsPolyline1.push_back(Point(2.2326706072004301, 8.8295788442703245));
//  polyline1.Init(refPointsPolyline1);
//
//  refPointsPolyline2.push_back(Point(0.077915099408919941, 9.4955925563173373));
//  refPointsPolyline2.push_back(Point(0.34658785599140246, 8.6728697355533804));
//  refPointsPolyline2.push_back(Point(0.74959699086512632, 9.3095004897159654));
//  refPointsPolyline2.push_back(Point(0.92154755507791508, 8.5357492654260536));
//  refPointsPolyline2.push_back(Point(1.2815690488984417, 9.2311459353574925));
//  refPointsPolyline2.push_back(Point(1.3729177861364859, 8.2713026444662106));
//  refPointsPolyline2.push_back(Point(1.969371305749597, 8.7610186092066602));
//  refPointsPolyline2.push_back(Point(2.184309511015583, 8.1831537708129289));
//  polyline2.Init(refPointsPolyline2);
//
//  refParams.clear();
//  points = SegmentsIntersections( &polyline1, &polyline2, refParams );
//  testpoints = IntersectPolylinePolyline(polyline1, polyline2);
//  EXPECT_TRUE(compare(points, testpoints));
//}

TEST(LineCircleIntersection, Intersection)
{
  Math::Ellipse ellipse(Math::Point(2., 2.), 2., 2., 0.);
  Math::Line line(Math::Point(0., 4.), Math::Point(4., 4.));

  // одна точка пересечения
  std::vector<Math::Point> point = Math::IntersectLineCircle(line, ellipse);
  EXPECT_TRUE(point.size() == 1);
  EXPECT_TRUE(IsEqual(point[0], Math::Point(2., 4.)));

  // две точки пересечения
  Math::Line line2(Math::Point(0., 4.), Math::Point(4., 0.));
  point = Math::IntersectLineCircle(line2, ellipse);
  EXPECT_TRUE(point.size() == 2);
  EXPECT_TRUE(IsEqual(point[0], Math::Point(3.41421356, 0.58578643762)));
  EXPECT_TRUE(IsEqual(point[1], Math::Point(0.58578643762690485, 3.4142135623)));

  // нет точек пересечения
  Math::Line line3(Math::Point(5., 4.), Math::Point(7., 8.));
  point = Math::IntersectLineCircle(line3, ellipse);
  EXPECT_TRUE(point.size() == 0);

  // начало отрезка - на границе окружности
  Math::Line line4(Math::Point(2., 4.), Math::Point(2., 8.));
  point = Math::IntersectLineCircle(line4, ellipse);
  EXPECT_TRUE(point.size() == 1);
  EXPECT_TRUE(IsEqual(point[0], Math::Point(2., 4.)));


  // Те же самые тесты, но пересечение находится не аналитически, а с помошью общего алгоритма.
  std::vector<Math::Point> pointsTest = Math::IntersectGeneralCase(line, ellipse);
  point = Math::IntersectLineCircle(line, ellipse);
  EXPECT_TRUE(compare(point, pointsTest));
}


TEST(Intersection, NurbsNurbsSym)
{
  std::vector<double> weights(5, 1.);
  std::vector<Math::Point> refPoints{ Math::Point(2., 2.), Math::Point(4., 5.), Math::Point(8.5, 7.),
    Math::Point(4., 9.), Math::Point(2., 12.) };

  Math::NurbsCurve nurbs1(refPoints, weights, false, 4);

  refPoints[0] = Math::Point(8., 2.); refPoints[1] = Math::Point(6., 5.); refPoints[2] = Math::Point(1.5, 7.);
  refPoints[3] = Math::Point(6., 9.); refPoints[4] = Math::Point(8., 12.);
  Math::NurbsCurve nurbs2(refPoints, weights, false, 4);
  auto intersections = Math::IntersectGeneralCase(nurbs1, nurbs2);
  EXPECT_EQ(intersections.size(), 2);
}
