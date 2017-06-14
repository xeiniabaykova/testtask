#include "EllipseCurveSerializer.h"

namespace Serializer {

//-----------------------------------------------------------------------------
//  Получение заголовка примитва эллпипс.
// ---
std::string EllipseCurveSerializer::GetHeaderName()
{
  return Math::Ellipse().GetName();
}


//-----------------------------------------------------------------------------
//  Чтение эллипса из потока. Данные читаются в следующем порядке: центр (Point), главный радиус, побочный радиус, угол наклона относительно оси ох.
// В случае невалидных данных возвращается пустой примитив.
// ---
std::unique_ptr<Math::Curve> EllipseCurveSerializer::Read( std::istream& input )
{
  Math::Point center = ReadPoint( input );
  double radius1 = ReadDouble( input );
  double radius2 = ReadDouble( input );
  double alpha = ReadDouble( input );
  return std::make_unique<Math::Ellipse>( center, radius1, radius2, alpha );
}


//-----------------------------------------------------------------------------
// Запись эллипса в поток. Данные записываются в следующем порядке: центр (Point), главный радиус, побочный радиус, угол наклона относительно оси ох.
// В случае невалидных данных в файл ничего не записывается.
// ---
void EllipseCurveSerializer::Write( std::ostream& output, const Math::Curve& curve )
{
  WritePoint( output, dynamic_cast<const Math::Ellipse&>(curve).GetCenter() );
  WriteDouble( output, dynamic_cast<const Math::Ellipse&>(curve).GetMajorRadius() );
  WriteDouble( output, dynamic_cast<const Math::Ellipse&>(curve).GetMinorRadius() );
  WriteDouble( output, dynamic_cast<const Math::Ellipse&>(curve).GetAlpha() );
}
}
