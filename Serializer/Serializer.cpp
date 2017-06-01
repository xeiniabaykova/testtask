//#include "Serializer.h"
//#include "BezierCurveSerializer.hxx"
//#include "CircleCurveSerializer.hxx"
//#include "EllipseCurveSerializer.h"
//#include "LineSerializer.h"
//#include <algorithm>
//#include <istream>
//#include <ostream>
//#include <fstream>
//#include <functional>
//#include <vector>


//namespace {

//	bool tryReadHeader (std::istream& theStream, FormattedCurveSerializer::Format theFormat, std::string& theHeader) {
//		if (theFormat == FormattedCurveSerializer::Format::Binary) {
//			int aLength;
//			theStream.read((char*)&aLength, sizeof(aLength));
//			if (theStream.eof())
//				return false;
//			char * aBuf = new char[aLength + 1];
//			theStream.read(aBuf, aLength);
//			aBuf[aLength] = '\0';
//			theHeader = std::string(aBuf);
//			delete[] aBuf;
//			return true;
//		}
//		return (bool)(theStream >> theHeader);
//	}

//	void WriteHeader(std::string theHeader, std::ostream& theStream, FormattedCurveSerializer::Format theFormat) {
//		if (theFormat == FormattedCurveSerializer::Format::Binary) {
//			size_t aLength = theHeader.length();
//			theStream.write((char*)&aLength, sizeof(aLength));
//			theStream.write(theHeader.c_str(), aLength);
//		}
//		else
//			theStream << theHeader << "\n";
//	}

//	std::string FileExtension(const std::string& aPath) {
//		size_t pos = aPath.rfind('.');
//		if (pos <= 0) return "";
//		return aPath.substr(pos + 1, std::string::npos);
//	}


//}


//CurveExchanger::CurveExchanger() {
//	RegisterSerializer<LineCurveSerializer>();
//	RegisterSerializer<CircleCurveSerializer>();
//	RegisterSerializer<EllipseCurveSerializer>();
//	RegisterSerializer<BezierCurveSerializer>();

//}

//template<typename FormattedSerializer>
//void CurveExchanger::RegisterSerializer() {
//	FormattedCurveSerializer::Format aDefaultFormat = FormattedCurveSerializer::Format::Text;
//	auto aSerializer = std::make_shared<FormattedSerializer>(aDefaultFormat);
//	std::string aHeaderName = aSerializer->GetHeaderName();
//	mySerializers[aHeaderName] = aSerializer;
//}

//std::unique_ptr<ICurve> CurveExchanger::Read(std::istream& theStream, FormattedCurveSerializer::Format theFormat) {
//	std::string aNameHeader;
//	bool isPresent = tryReadHeader(theStream, theFormat, aNameHeader);
//	if (!isPresent)
//		return nullptr;
//	auto aSerializer = mySerializers.at(aNameHeader);
//	aSerializer->setFormat(theFormat);
//	return aSerializer->Read(theStream);
//}
//void CurveExchanger::Write(std::ostream& theStream, const ICurve& aCurve, FormattedCurveSerializer::Format theFormat) {
//	std::string aNameHeader = aCurve.GetName();
//	auto aSerializer = mySerializers.at(aNameHeader);
//	aSerializer->setFormat(theFormat);
//	WriteHeader(aNameHeader, theStream, theFormat);
//	aSerializer->Write(theStream, aCurve);
//}

//std::vector<std::shared_ptr<ICurve>> CurveExchanger::ReadCurves (std::string theFileName) {
//	std::ifstream aInputFile;
//	FormattedCurveSerializer::Format aFormat;
//	if (FileExtension(theFileName) == "bin") {
//		aInputFile.open(theFileName, std::ios::binary);
//		aFormat = FormattedCurveSerializer::Format::Binary;
//	}
//	if (FileExtension(theFileName) == "txt") {
//		aInputFile.open(theFileName);
//		aFormat = FormattedCurveSerializer::Format::Text;
//	}
//	std::vector<std::shared_ptr<ICurve>> aCurveVector;
//	while (!aInputFile.eof()) {
//		std::shared_ptr<ICurve> aCurve = Read(aInputFile, aFormat);
//		if (aCurve)
//			aCurveVector.push_back(aCurve);
//	}
//	return aCurveVector;
//}

//void CurveExchanger::WriteCurves (const std::vector<std::shared_ptr<ICurve>>& theCurves, std::string theFileName) {

//	std::function<bool(std::shared_ptr<ICurve>, std::shared_ptr<ICurve>)> aCompare =
//		[] (std::shared_ptr<ICurve> aCurveFirst, std::shared_ptr<ICurve> aCurveSecond) {
//		return *aCurveFirst == *aCurveSecond;
//	};
//	std::vector<std::shared_ptr<ICurve>> aUniqueCurves = theCurves;
//	auto aIt = std::unique (aUniqueCurves.begin(), aUniqueCurves.end(), aCompare);
//	aUniqueCurves.erase(aIt, aUniqueCurves.end());
//	std::ofstream fout;
//	fout.open(theFileName, std::ios::binary);
//	for (size_t i = 0; i < aUniqueCurves.size(); i++)
//		Write(fout, *aUniqueCurves[i].get(), FormattedCurveSerializer::Format::Binary);
//}
