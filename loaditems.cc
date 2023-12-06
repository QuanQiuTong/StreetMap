#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "load.h"
#include "Items.h"

#include "osm.h"
using namespace osm; // only in this file

static double scale = 1 << 18;
static double radius = 2e-5;

static void loadWay(QGraphicsScene *scene, const Way &way)
{
    if (way.nd.front() == way.nd.back())
    {
        // closed way
        QPolygonF polygon;
        for (auto &&node : way.nd)
            polygon << QPointF(node.lon * scale, node.lat * -scale);
        if (way.tag.count("building"))
        {
            Building *building = new Building(polygon);
            scene->addItem(building);
        }
        else
        {
            scene->addPolygon(polygon);
        }
    }
    else if (way.tag.count("highway"))
    {
        // open way
        QGraphicsPathItem *path = new QGraphicsPathItem;
        QPainterPath painterPath;
        // Draw by segments
        for (size_t i = 0; i < way.nd.size() - 1; i++)
        {
            auto node = way.nd[i];
            auto next = way.nd[i + 1];
            painterPath.moveTo(node.lon * scale, node.lat * -scale);
            painterPath.lineTo(next.lon * scale, next.lat * -scale);
            scene->addItem(new WayPoint(node.lon * scale, node.lat * -scale, radius * scale));
        }
        path->setPath(painterPath);
        scene->addItem(path);
    }

    // else scene->addPolygon(polygon);
    //  else if (way.tag["highway"] == "footway")
    //  {
    //      QGraphicsPathItem *path = new QGraphicsPathItem;
    //      QPainterPath painterPath;
    //      for (auto node : way.nd)
    //          painterPath.lineTo(node.lon * scale, node.lat * -scale);
    //      path->setPath(painterPath);
    //      scene->addItem(path);
    //  }
}

static void loadOSM(QGraphicsScene *scene)
{
    scene->setSceneRect(minlon * scale, maxlat * -scale, (maxlon - minlon) * scale, (maxlat - minlat) * scale);

    for (auto [id, way] : ways)
        loadWay(scene, way);
}

void loadScene(QGraphicsScene *scene, const std::string &filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        throw std::invalid_argument("File not found");
    parse(fp);
    //printf("%zu %zu %zu\n", nodes.size(), ways.size(), relations.size());
    loadOSM(scene);
}
