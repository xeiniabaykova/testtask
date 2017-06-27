#include "Range.h"
#include <cmath>

namespace Math {
//-----------------------------------------------------------------------------
//  Конструктор Range.
// ----
Range::Range( double start, double end ):
  start( start ),
  end  ( end   )
{
}


//-----------------------------------------------------------------------------
//  Конструктор копирования .
// ---
Range::Range(const Range& range)
{
  start = range.GetStart();
  end = range.GetEnd();
}


//-----------------------------------------------------------------------------
//  Возращаем начало интервала.
// ---
double Range::GetStart() const
{
  return start;
}


//-----------------------------------------------------------------------------
//  Возращаем конец интервала.
// ---
double Range::GetEnd() const
{
  return end;
}

//-----------------------------------------------------------------------------
// Возращаем корректность интервала.
// ---
bool Range::IsValid() const
{
   return !( std::isinf(start) || std::isinf(end) );
}


//-----------------------------------------------------------------------------
// Размер интервала.
// ---
double Range::Lenght() const
{
  return end - start;
}


void Range::SetRange( double theStart, double theEnd )
{
  start = theStart;
  end = theEnd;
}
}
