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
//-----------------------------------------------------------------------------
//  Чтение заголовка для типа данных. Если достигнут конец файла, то возвращается false.
// ---
static bool TryReadHeader( std::istream& stream, std::string& header )
{
  size_t length;
  stream.read( (char*)&length, sizeof(length) );
  if ( stream.eof() )
    return false;
  char *buf = new char[length + 1];
  stream.read( buf, length );
  buf[length] = '\0';
  header = std::string( buf );
  delete[] buf;

  return true;
}


//-----------------------------------------------------------------------------
//  Запись заголовка для типа данных в файл.
// ---
static void WriteHeader( std::string header, std::ofstream& stream )
{
  const size_t length = header.length();
  stream.write( (char*)&length, sizeof(length) );
  stream.write( header.c_str(), length );
}
}

//-----------------------------------------------------------------------------
//  Созадание читателей и писателей для следующих типов: отрезок, эллипс, Nurbs - кривая.
// ---
Serializer::Serializer()
{
  RegisterSerializer<LineSerializer>();
  RegisterSerializer<EllipseCurveSerializer>();
  RegisterSerializer<NURBSCurveSerializer>();
}


//-----------------------------------------------------------------------------
//  Созадание читаелея / писателя для типа данных.
// ---
template<typename CurveSerializer>
void Serializer::RegisterSerializer()
{
  const auto serializer = std::make_shared<CurveSerializer>();
  const std::string headerName = serializer->GetHeaderName();
  mySerializers[headerName] = serializer;
}


//-----------------------------------------------------------------------------
//  Чтение кривой из потока. Читается заголовок, затем вызывается нужный читатель для типа.
// ---
std::unique_ptr<Math::Curve> Serializer::Read( std::istream& stream ) const
{
  std::string nameHeader;
  const bool isPresent = TryReadHeader( stream, nameHeader );
  if ( !isPresent )
    return nullptr;
  const auto serializer = mySerializers.find( nameHeader );
  if ( serializer == mySerializers.end() )
    return nullptr;
  return serializer->second->Read( stream );
}


//-----------------------------------------------------------------------------
//  Запись кривой в поток. Сначала записывается заголовок, затем вызывается нужный писатель для типа.
// ---
void Serializer::Write( std::ofstream& stream, const Math::Curve& curve ) const
{
  std::string nameHeader = curve.GetName();
  const auto serializer = mySerializers.find( nameHeader );
  if ( serializer != mySerializers.end() )
  {
    WriteHeader( nameHeader, stream );
    serializer->second->Write( stream, curve );
  }
}


//-----------------------------------------------------------------------------
//  Чтение кривых из потока. Файл открывается в бинарном формате, кривые читаются в порядке нахождения в файле.
// ---
std::vector<std::shared_ptr<Math::Curve>> Serializer::ReadCurves( std::string fileName ) const
{
  std::ifstream inputFile;
    inputFile.open( fileName, std::ios::binary );

  std::vector<std::shared_ptr<Math::Curve>> curveVector;
  while ( !inputFile.eof() )
  {
    std::shared_ptr<Math::Curve> curve = Read( inputFile );
    if ( curve )
      curveVector.push_back(curve);
  }
  return curveVector;
}


//-----------------------------------------------------------------------------
//  Запись кривых в поток. Файл открывается в бинарном формате, кривые записываеются в порядке нахождения в curves.
// ---
void Serializer::WriteCurves( const std::vector<std::shared_ptr<Math::Curve>>& curves, std::string fileName ) const
{
  std::ofstream fout;
  fout.open( fileName, std::ios::binary );
  for ( size_t i = 0; i < curves.size(); i++ )
    Write( fout, *curves[i] );
  fout.close();
}
}
