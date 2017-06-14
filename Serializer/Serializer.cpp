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
static bool tryReadHeader( std::istream& stream, std::string& header )
{
  int length;
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
  int length = header.length();
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
  auto serializer = std::make_shared<CurveSerializer>();
  std::string headerName = serializer->GetHeaderName();
  mySerializers[headerName] = serializer;
}


//-----------------------------------------------------------------------------
//  Чтение кривой из потока. Читается заголовок, затем вызывается нужный читатель для типа.
// ---
std::unique_ptr<Math::Curve> Serializer::Read( std::istream& stream )
{
  std::string nameHeader;
  bool isPresent = tryReadHeader( stream, nameHeader );
  if ( !isPresent )
    return nullptr;
  auto serializer = mySerializers.at( nameHeader );
  return serializer->Read( stream );
}


//-----------------------------------------------------------------------------
//  Запись кривой в поток. Сначала записывается заголовок, затем вызывается нужный писатель для типа.
// ---
void Serializer::Write( std::ofstream& stream, const Math::Curve& curve )
{
  std::string nameHeader = curve.GetName();
  auto serializer = mySerializers.at( nameHeader );
  WriteHeader( nameHeader, stream );
  serializer->Write(stream, curve);
}


//-----------------------------------------------------------------------------
//  Чтение кривых из потока. Файл открывается в бинарном формате, кривые читаются в порядке нахождения в файле.
// ---
std::vector<std::shared_ptr<Math::Curve>> Serializer::ReadCurves( std::string fileName )
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
void Serializer::WriteCurves( const std::vector<std::shared_ptr<Math::Curve>>& curves, std::string fileName )
{
  std::ofstream fout;
  fout.open( fileName, std::ios::binary );
  for ( size_t i = 0; i < curves.size(); i++ )
    Write( fout, *curves[i].get() );
  fout.close();
}
}
