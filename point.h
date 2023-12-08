#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <QPointF>
#include "osm.h"

class Point
{
    using ll = long long;
    double x, y;
public:
    static constexpr double xScale = 1 << 18, yScale = -(1 << 18);
    Point() = default;
    Point(const osm::Node &node): x(node.lon *xScale), y(node.lat *yScale) {}
    Point(ll id)
    {
        if (!osm::nodes.count(id))
            throw;
        x = osm::nodes[id].lon * xScale, y = osm::nodes[id].lat * yScale;
    }

    operator QPointF()const {return {x,y};}

    bool operator<(const Point &rhs) const { return x < rhs.x ||( x == rhs.x && y < rhs.y); }
    friend double distance(const Point &lhs, const Point &rhs) { return std::hypot(lhs.x - rhs.x, lhs.y - rhs.y); }
};

#endif // POINT_H
