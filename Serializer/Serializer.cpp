#include "Serializer.h"
#include "EllipseCurveSerializer.h"
#include "NURBSCurveSerializer.h"
#include "LineSerializer.h"
#include <algorithm>
#include <istream>
#include <ostream>
#include <fstream>
#include <functional>
#include <vector>

namespace Serializer {
namespace {

static bool tryReadHeader( std::istream& theStream, std::string& theHeader )
{
  int length;
  theStream.read( (char*)&length, sizeof(length) );
  if ( theStream.eof() )
    return false;
  char * aBuf = new char[length + 1];
  theStream.read( aBuf, length );
  aBuf[length] = '\0';
  theHeader = std::string(aBuf);
  delete[] aBuf;
  return true;

}

static void WriteHeader( std::string theHeader, std::ostream& theStream )
{

  size_t length = theHeader.length();
  theStream.write( (char*)&length, sizeof(length) );
  theStream.write( theHeader.c_str(), length );

}

static std::string FileExtension( const std::string& path )
{
  size_t pos = path.rfind( '.' );
  if ( pos <= 0 )
    return "";
  return path.substr( pos + 1, std::string::npos );
}
}


Serializer::Serializer()
{
  RegisterSerializer<LineSerializer>();
  RegisterSerializer<EllipseCurveSerializer>();
  RegisterSerializer<NURBSCurveSerializer>();

}

template<typename CurveSerializer>
void Serializer::RegisterSerializer()
{
  auto aSerializer = std::make_shared<CurveSerializer>();
  std::string aHeaderName = aSerializer->GetHeaderName();
  mySerializers[aHeaderName] = aSerializer;
}

std::unique_ptr<Math::GeometricPrimitive> Serializer::Read( std::istream& theStream )
{
  std::string aNameHeader;
  bool isPresent = tryReadHeader( theStream, aNameHeader );
  if ( !isPresent )
    return nullptr;
  auto aSerializer = mySerializers.at( aNameHeader );
  return aSerializer->Read( theStream );
}
void Serializer::Write( std::ostream& theStream, const Math::GeometricPrimitive& aCurve )
{
  std::string aNameHeader = aCurve.GetName();
  auto aSerializer = mySerializers.at( aNameHeader );
  WriteHeader( aNameHeader, theStream );
  aSerializer->Write(theStream, aCurve);
}

std::vector<std::shared_ptr<Math::GeometricPrimitive>> Serializer::ReadCurves (std::string theFileName)
{
  std::ifstream aInputFile;
    aInputFile.open(theFileName, std::ios::binary);


  std::vector<std::shared_ptr<Math::GeometricPrimitive>> aCurveVector;
  while (!aInputFile.eof()) {
    std::shared_ptr<Math::GeometricPrimitive> aCurve = Read( aInputFile );
    if (aCurve)
      aCurveVector.push_back(aCurve);
  }
  return aCurveVector;
}

void Serializer::WriteCurves (const std::vector<std::shared_ptr<Math::GeometricPrimitive>>& theCurves, std::string theFileName)
{

  std::function<bool(std::shared_ptr<Math::GeometricPrimitive>, std::shared_ptr<Math::GeometricPrimitive>)> aCompare =
    [] (std::shared_ptr<Math::GeometricPrimitive> aCurveFirst, std::shared_ptr<Math::GeometricPrimitive> aCurveSecond) {
    return *aCurveFirst == *aCurveSecond;
  };
  std::vector<std::shared_ptr<Math::GeometricPrimitive>> aUniqueCurves = theCurves;
  auto aIt = std::unique( aUniqueCurves.begin(), aUniqueCurves.end(), aCompare );
  aUniqueCurves.erase( aIt, aUniqueCurves.end() );
  std::ofstream fout;
  fout.open( theFileName, std::ios::binary );
  for ( size_t i = 0; i < aUniqueCurves.size(); i++ )
    Write( fout, *aUniqueCurves[i].get() );
}
}
