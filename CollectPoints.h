#ifndef COLLECTPOINTS_H
#define COLLECTPOINTS_H

#include <QPoint>
#include <vector>
class CollectPoints
{
public:
    bool isSufficient;
    int  sufficient;
    std::vector<QPoint> points;
    CollectPoints();
};

#endif // COLLECTPOINTS_H
