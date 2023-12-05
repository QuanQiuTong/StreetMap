#include <QGraphicsScene>

#include "load.h"
#include "Items.h"

#include "osm.h"
using namespace osm; // only in this file

static void loadOSM(QGraphicsScene *scene, double scale)
{
    auto equal = [](const Node &a, const Node &b)
    { return a.lon == b.lon && a.lat == b.lat; };

    double x = minlon * scale, y = maxlat * -scale, w = (maxlon - minlon) * scale, h = (maxlat - minlat) * scale;
    scene->setSceneRect(x, y, w, h);

    for (auto [id, way] : ways)
    {
        QPolygonF polygon;
        if (equal(way.nd.front(), way.nd.back()))
            for (auto node : way.nd)
                polygon << QPointF(node.lon * scale, node.lat * -scale);

        if (way.tag["building"] == "yes")
        {
            Building *building = new Building(polygon);
            scene->addItem(building);
        }
        else
            scene->addPolygon(polygon);
        // else if (way.tag["highway"] == "footway")
        // {
        //     QGraphicsPathItem *path = new QGraphicsPathItem;
        //     QPainterPath painterPath;
        //     for (auto node : way.nd)
        //         painterPath.lineTo(node.lon * scale, node.lat * -scale);
        //     path->setPath(painterPath);
        //     scene->addItem(path);
        // }
    }
}

void loadScene(QGraphicsScene *scene)
{
    FILE *fp = fopen("C:\\Users\\fqt15\\Documents\\StreetMap\\map.osm", "r");
    if (!fp)
        throw std::invalid_argument("File not found");
    parse(fp);
    loadOSM(scene, 1 << 18);
}
