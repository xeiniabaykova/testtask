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
  double alphaMajor = ReadDouble( input );
  double alphaMinor = ReadDouble( input );
  return std::make_unique<Math::Ellipse>( center, radius1, radius2, alphaMajor, alphaMinor  );
}


//-----------------------------------------------------------------------------
// Запись эллипса в поток. Данные записываются в следующем порядке: центр (Point), главный радиус, побочный радиус, угол наклона относительно оси ох.
// В случае невалидных данных в файл ничего не записывается.
// ---
void EllipseCurveSerializer::Write( std::ostream& output, const Math::Curve& curve )
{
  const Math::Ellipse* ellipseCurve = dynamic_cast<const Math::Ellipse*>( &curve );
  if ( ellipseCurve != nullptr )
  {
    WritePoint( output, ellipseCurve->GetCenter() );
    WriteDouble( output, ellipseCurve->GetMajorRadius() );
    WriteDouble( output, ellipseCurve->GetMinorRadius() );
    WriteDouble( output, ellipseCurve->GetAlphaMajorAxis() );
    WriteDouble( output, ellipseCurve->GetAlphaMinorAxis());
  }

}
}
