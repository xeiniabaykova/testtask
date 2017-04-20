#ifndef COLLECTPOINTS_H
#define COLLECTPOINTS_H

#include <QPoint>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
/// Класс хранения точек, полученных с экрана
/**
*/
///////////////////////////////////////////////////////////////////////////////
///
class CollectPoints
{
public:
    std::vector<QPoint> points;
    CollectPoints();
};

#endif // COLLECTPOINTS_H
