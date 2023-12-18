#include <unordered_set>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "Items.h"
#include "path.h"
#include "osm.h"
#include "point.h"
#include "load.h"

using namespace osm; // only in this file

std::unordered_set<ll> waypoints;
long long nearestPoint(Point point)
{
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](ll a, ll b)
                             { return _dist(point, nodes.at(a)) < _dist(point, nodes.at(b)); });
}

void loadClosedWay(const Way &way)
{
    QPolygonF polygon;
    for (auto nd : way.nd)
        polygon << Point(nd);
    if (way.tag.count("building"))
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(Qt::lightGray));
    else if (way.tag.count("landuse"))
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(QColor(144, 238, 144)));
    else
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(QColor(238, 238, 221)));
}

void loadOpenWay(const Way &way)
{
    auto it = way.nd.begin();
    QPainterPath painterPath{Point(*it)};
    waypoints.insert(*it);
    while (++it != way.nd.end())
    {
        painterPath.lineTo(Point(*it));
        waypoints.insert(*it);
        path::addEdge(*it, *(it - 1));
    }
    scene->addItem(new QGraphicsPathItem(painterPath));
}

bool loadScene(const std::string &filename)
{
    if (!parse(filename))
        return false;
#if VISIBLE
    visible.clear();
#endif
    path::SRC = path::DST = path::srcPos = path::dstPos = Point();
    path::path.clear();
    scene->clear();
    waypoints.clear();

    scene->setSceneRect(minlon * Point::xScale, maxlat * Point::yScale, (maxlon - minlon) * Point::xScale, (minlat - maxlat) * Point::yScale);
    for (auto &&w : closedways)
        loadClosedWay(w);
    for (auto &&w : openways)
        loadOpenWay(w);
    closedways.clear(), openways.clear();
    for (auto id : waypoints)
        scene->addItem(new WayPoint(Point(id), id));
    return true;
}
