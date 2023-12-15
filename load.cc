#include <unordered_set>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "Items.h"
#include "path.h"
#include "osm.h"
#include "point.h"

using namespace osm; // only in this file

std::unordered_set<ll> waypoints;
long long nearestPoint(Point point)
{
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](ll a, ll b)
                             { return distance(point, nodes.at(a)) < distance(point, nodes.at(b)); });
}

static void loadClosedWay(const Way &way)
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

static void loadOpenWay(const Way &way)
{
    if(!way.tag.count("highway"))
        return;
    QPainterPath painterPath;
    auto it = way.nd.begin();
    painterPath.moveTo(Point(*it));
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
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        return false;
    parse(fp); // printf("%zu %zu %zu\n", nodes.size(), ways.size(), relations.size());
    fclose(fp);

#if VISIBLE
    visible.clear();
#endif
    path::SRC = path::DST = path::srcPos = path::dstPos = Point();
    path::path.clear();
    scene->clear();
    scene->setSceneRect(minlon * Point::xScale, maxlat * Point::yScale, (maxlon - minlon) * Point::xScale, (minlat - maxlat) * Point::yScale);
    waypoints.clear();

    for (auto &&p : closedways)
        loadClosedWay(p.second);
    for (auto &&p : openways)
        loadOpenWay(p.second);
    for (auto id : waypoints)
        scene->addItem(new WayPoint(Point(id), id));
    return true;
}
