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
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](auto a, auto b)
                        { return distance(point, a) < distance(point, b); });
}

static void loadWay(QGraphicsScene *scene, const Way &way)
{
    const auto &[nd, tag] = way;
    if (nd.front() == nd.back()) // closed way
    {
        QPolygonF polygon;
        for (auto nd : nd)
            polygon << Point(nd);
        if (tag.count("building"))
            scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(Qt::lightGray));
        else if(tag.count("landuse"))
            scene->addPolygon(polygon, QPen(Qt::darkGreen), QBrush(Qt::green));
        else
            scene->addPolygon(polygon);
    }
    else if (tag.count("highway") || tag.count("")) // open way
    {
        QPainterPath painterPath;
        auto it = nd.begin();
        painterPath.moveTo(Point(*it));
        waypoints.insert(*it);
        while (++it != nd.end())
        {
            painterPath.lineTo(Point(*it));
            waypoints.insert(*it);
            path::addEdge(*it, *(it - 1));
        }
        scene->addItem(new QGraphicsPathItem(painterPath));
    }
}

bool loadScene(QGraphicsScene *scene, const std::string &filename)
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
    for (auto &&p : ways)
        loadWay(scene, p.second);

    for (auto id : waypoints)
        scene->addItem(new WayPoint(Point(id), id));

    pscene = scene;
    return true;
}
