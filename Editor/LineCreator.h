#ifndef LINECREATOR_H
#define LINECREATOR_H
#include "GeometricPrimitiveCreator.h"

QT_CHARTS_USE_NAMESPACE


//------------------------------------------------------------------------------
/** \brief \ru Класс для отображения линни в GUI. \~
*/
// ---
class LineCreator: public GeometricPrimitiveCreator {
public:
  void Create( QChart * chart, const std::vector<QPoint>& points );
};

#endif // LINECREATOR_H
