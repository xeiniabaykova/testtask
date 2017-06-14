#include "LineSerializer.h"


namespace Serializer {
//-----------------------------------------------------------------------------
//  Получение заголовка примитва отрезок.
// ---
std::string LineSerializer::GetHeaderName()
{
  return Math::Line( Math::Point(), Math::Point() ).GetName();
}


//-----------------------------------------------------------------------------
//  Чтение отрезка из потока. Данные читаются в следующем порядке: начало отрезка (Point), конец отрезка (Point).
// В случае невалидных данных возвращается пустой примитив.
// ---
std::unique_ptr<Math::Curve> LineSerializer::Read( std::istream& input )
{
  Math::Point point1;
  Math::Point point2;
  point1 = ReadPoint( input );
  point2 = ReadPoint( input );
  return std::make_unique<Math::Line>( point1, point2 );
}


//-----------------------------------------------------------------------------
//  Запись отрезка в поток. Данные записываются в следующем порядке: начало отрезка (Point), конец отрезка (Point).
// В случае невалидных данных в файл ничего не записывается.
// ---
void LineSerializer::Write( std::ostream& output, const Math::Curve& curve )
{
  WritePoint( output, dynamic_cast<const Math::Line&> (curve).GetStartPoint() );
  WritePoint( output, dynamic_cast<const Math::Line&> (curve).GetEndPoint() );
}
}
