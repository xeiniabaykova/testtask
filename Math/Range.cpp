#include "Range.h"


//-----------------------------------------------------------------------------
/**
  Конструктор Range
*/
//-----------------------------------------------------------------------------
Range::Range( double start, double end ):
  start( start ),
  end  ( end   )
{
}


//-----------------------------------------------------------------------------
/**
  Конструктор копирования Point
*/
//-----------------------------------------------------------------------------
Range::Range(const Range& range)
{
  start = range.GetStart();
  end = range.GetEnd();
}


//-----------------------------------------------------------------------------
/**
  возращаем начало интервала
*/
//-----------------------------------------------------------------------------
double Range::GetStart() const
{
  return start;
}


//-----------------------------------------------------------------------------
/**
  возращаем конец интервала
*/
//-----------------------------------------------------------------------------
double Range::GetEnd() const
{
  return end;
}
