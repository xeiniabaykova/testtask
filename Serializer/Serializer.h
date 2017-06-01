//#ifndef SERIALIZER_H
//#define SERIALIZER_H
//#include "CurveSerializer.h"
//#include "Math/GeometricPrimitive.h"
//#include <memory>
//#include <string>
//#include <vector>
//namespace Serializer {
//class Serializer {
//public:
//	Serializer();
//	void RegisterSerializer();

//	std::unique_ptr<GeometricPrimitive> Read ( std::istream& theStream );
//	void Write (std::ostream& theStream, const GeometricPrimitive& aCurve );

//	std::vector<std::shared_ptr<GeometricPrimitive>> ReadCurves ( std::string theFileName );
//	void WriteCurves (const std::vector<std::shared_ptr<GeometricPrimitive>>& theCurves,std::string theFileName);

//private:

//	std::map<std::string, std::shared_ptr<CurveSerializer> > mySerializers;


//};
//}
//#endif // SERIALIZER_H
