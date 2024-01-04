/**
 * @file load.cc
 * @brief Implementation of the loadScene function.
 */

#include <unordered_set>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "Items.h"
#include "path.h"
#include "osm.h"
#include "point.h"

using namespace osm;

std::unordered_set<ll> waypoints;
/**
 * @brief Find the nearest point.
 * 
 * This function finds the nearest point to the given target point.
 * 
 * @param point The target point.
 * @return long long The index of the nearest point.
 */
long long nearestPoint(Point point)
{
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](ll a, ll b)
                             { return distance(point, nodes.at(a)) < distance(point, nodes.at(b)); });
}

/**
 * @brief Loads a closed way into the scene as a polygon.
 * 
 * @param way The Way object representing the closed way to be loaded.
 */
static void loadClosedWay(const Way &way)
{
    QPolygonF polygon;
    for (auto nd : way.nd)
        polygon << Point(nd);
    if (way.tag.count("building"))
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(Qt::lightGray));
    else if (way.tag.count("landuse"))
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(QColor(192, 244, 192)));
    else
        scene->addPolygon(polygon, QPen(Qt::darkGray), QBrush(QColor(238, 238, 221)));
}

/**
 * @brief Loads an open way into the scene.
 * 
 * @param way The Way object representing the open way to be loaded.
 */
static void loadOpenWay(const Way &way)
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

/**
 * @brief Loads a scene from a file.
 * 
 * This function loads a scene from the specified file. It opens the file, parses its contents, and populates the scene with the data.
 * 
 * @param filename The name of the file to load the scene from.
 * @return True if the scene was successfully loaded, false otherwise.
 */
bool loadScene(const std::string &filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        return false;
    parse(fp);
    fclose(fp);

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
    closedways.clear();
    for (auto &&w : openways)
        loadOpenWay(w);
    openways.clear();
    for (auto id : waypoints)
        scene->addItem(new WayPoint(Point(id), id));
    return true;
}
