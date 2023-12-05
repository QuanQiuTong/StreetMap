#include <QGraphicsScene>

#include "load.h"

#include "osm.h"
using namespace osm; // only in this file

template <template <typename Key, typename Val> typename AssocCon>
static void loadOSM(QGraphicsScene *scene,
                    double scale,
                    Bounds bound,
                    const AssocCon<ll, Node> &nodes,
                    const AssocCon<ll, Way> &ways,
                    const AssocCon<ll, Relation> &relations)
{
    auto equal = [](const Node &a, const Node &b)
    { return a.lon == b.lon && a.lat == b.lat; };

    double x = bound.minlon * scale, y = bound.maxlat * scale, w = (bound.maxlon - bound.minlon) * scale, h = (bound.maxlat - bound.minlat) * scale;
    scene->setSceneRect(x-w/4, y-w, w*1.5, h*2.25);

    for (auto [id, way] : ways)
    {
        QPolygonF polygon;
        if (equal(way.nd.front(), way.nd.back()))
            for (auto node : way.nd)
                polygon << QPointF(node.lon * scale, node.lat * scale);

        scene->addPolygon(polygon);
    }

    // make the compiler happy.
    nodes;
    relations;
}

void loadScene(QGraphicsScene *scene)
{
    FILE *fp = fopen("C:\\Users\\fqt15\\Documents\\StreetMap\\map.osm", "r");
    if (!fp)
        throw std::invalid_argument("File not found");
    std::map<ll, Node> nodes;
    std::map<ll, Way> ways;
    std::map<ll, Relation> relations;
    Bounds bound = parse(fp, nodes, ways, relations);
    loadOSM(scene, 1<<18, bound, nodes, ways, relations);
}
