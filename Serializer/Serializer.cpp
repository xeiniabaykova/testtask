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
  char *aBuf = new char[length + 1];
  for ( size_t i = 0; i < length +1; i++)
    aBuf[i]='1';
  theStream.read( aBuf, length );
  aBuf[length] = '\0';
  theHeader = std::string(aBuf);

  delete[] aBuf;
  return true;

}

static void WriteHeader( std::string theHeader, std::ofstream& theStream )
{
  int length = theHeader.length();
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

std::unique_ptr<Math::Curve> Serializer::Read( std::istream& theStream )
{
  std::string aNameHeader;
  bool isPresent = tryReadHeader( theStream, aNameHeader );
  if ( !isPresent )
    return nullptr;
  auto aSerializer = mySerializers.at( aNameHeader );
  return aSerializer->Read( theStream );
}
void Serializer::Write( std::ofstream& theStream, const Math::Curve& aCurve )
{

  std::string aNameHeader = aCurve.GetName();
  auto aSerializer = mySerializers.at( aNameHeader );
  WriteHeader( aNameHeader, theStream );
  aSerializer->Write(theStream, aCurve);
}

std::vector<std::shared_ptr<Math::Curve>> Serializer::ReadCurves (std::string theFileName)
{
  std::ifstream aInputFile;
    aInputFile.open(theFileName, std::ios::binary);

  std::vector<std::shared_ptr<Math::Curve>> aCurveVector;
  while (!aInputFile.eof()) {
    std::shared_ptr<Math::Curve> aCurve = Read( aInputFile );
    if (aCurve)
      aCurveVector.push_back(aCurve);
  }
  return aCurveVector;
}

void Serializer::WriteCurves (const std::vector<std::shared_ptr<Math::Curve>>& theCurves, std::string theFileName)
{
  std::ofstream fout;
  fout.open( theFileName, std::ios::binary );
  for ( size_t i = 0; i < theCurves.size(); i++ )
    Write( fout, *theCurves[i].get() );
  fout.close();
}
}
