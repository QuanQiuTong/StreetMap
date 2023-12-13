#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <map>
#include <QPointF>
#include "osm.h"

static constexpr double R = 1 << 18;
using ll = long long;
struct Point
{
    double x, y;
    static constexpr double xScale = 0.8544 * R, yScale = -R;
    Point() = default;
    Point(osm::Node node) : x(node.lon * xScale), y(node.lat * yScale) {}
    // This is WRONG!!!!!!!!!!!!//Point(QPointF qPointF) : x(qPointF.x() * xScale), y(qPointF.y() * yScale) {}
    Point(QPointF qPointF) : x(qPointF.x()), y(qPointF.y()) {}
    Point(ll id)
    {
        auto [lon, lat] = osm::nodes.at(id); //'at' is const and will throw an exception if id is not found
        x = lon * xScale, y = lat * yScale;
    }

    operator QPointF() const { return {x, y}; }

    bool operator<(const Point &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
    friend double distance(const Point &lhs, const Point &rhs) { return hypot(lhs.x - rhs.x, lhs.y - rhs.y); }
};

struct idPoint : public Point
{
public:
    long long id;
    idPoint() = default;
    idPoint(long long id) : Point(id), id(id) {}
    idPoint(osm::Node node) = delete; // write later
    idPoint(QPointF qPointF) : Point(qPointF) {}
    operator long long() const { return id; }
    operator QPointF() const { return {x, y}; }
    bool operator<(const idPoint &rhs) const { return id < rhs.id; }
};
#endif // POINT_H
