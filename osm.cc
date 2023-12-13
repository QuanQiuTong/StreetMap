#include <stdexcept>

// Directly included for they cannot be compiled by Make correctly.
#include "tinyxml.cpp"
#include "tinyxmlparser.cpp"
#include "tinyxmlerror.cpp"
#define forSibling(p, t) for (; p && ((TiXmlNode *)p)->ValueTStr() == t; p = p->NextSiblingElement())
static const char *att(TiXmlElement *p, const char *attr)
{
    const char *ret = p->Attribute(attr); // printf("# %s - %s\n", attr, ret);
    if (!ret)
        throw std::invalid_argument("Attribute " + std::string(attr) + " not found");
    return ret;
}
static double attd(TiXmlElement *p, const char *attr) { return atof(att(p, attr)); }
static long long id(TiXmlElement *p) { return atoll(att(p, "id")); }
static long long ref(TiXmlElement *p) { return atoll(att(p, "ref")); }

#include "osm.h"

NAMESPACE_OSM

double minlat, minlon, maxlat, maxlon;
AssocCon<ll, Node> nodes;
AssocCon<ll, Way> ways;
AssocCon<ll, Relation> relations;
void parse(const std::string &filename)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(filename))
        throw std::invalid_argument("File not found");
    TiXmlElement *p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    minlat = attd(p, "minlat"), minlon = attd(p, "minlon"), maxlat = attd(p, "maxlat"), maxlon = attd(p, "maxlon");
    p = p->NextSiblingElement();
    nodes.clear(), ways.clear(), relations.clear();
    forSibling(p, "node") nodes.insert({id(p), {attd(p, "lon"), attd(p, "lat")}});
    forSibling(p, "way")
    {
        auto &w = ways[id(p)];
        TiXmlElement *q = p->FirstChildElement();
        forSibling(q, "nd") w.nd.push_back(ref(q));
        forSibling(q, "tag") w.tag.insert({att(q, "k"), att(q, "v")});
    }
    forSibling(p, "relation")
    {
        auto &r = relations[id(p)];
        TiXmlElement *q = p->FirstChildElement();
        forSibling(q, "member") r.member.push_back({att(q, "type"), ref(q), att(q, "role")});
        forSibling(q, "nd") r.tag.insert({att(q, "k"), att(q, "v")});
    }
}

END_NAMESPACE_OSM

#include <set>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "Items.h"
#include "path.h"
#include "osm.h"
#include "point.h"

using namespace osm; // only in this file

std::set<ll> waypoints;
static void loadWay(QGraphicsScene *scene, const Way &way)
{
    if (way.nd.front() == way.nd.back()) // closed way
    {
        QPolygonF polygon;
        for (auto nd : way.nd)
            polygon << Point(nd);
        if (way.tag.count("building"))
            scene->addItem(new Building(polygon));
        else
            scene->addPolygon(polygon);
    }
    else if (way.tag.count("highway")) // open way
    {
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
}

void loadScene(QGraphicsScene *scene, const std::string &filename)
{
    parse(filename); // printf("%zu %zu %zu\n", nodes.size(), ways.size(), relations.size());

    path::SRC = path::DST = 0;
    path::path.clear();
    scene->clear();
    scene->setSceneRect(minlon * Point::xScale, maxlat * Point::yScale, (maxlon - minlon) * Point::xScale, (minlat - maxlat) * Point::yScale);

    waypoints.clear();
    for (auto&& p : ways)
        loadWay(scene, p.second);

    for (auto id : waypoints)
        scene->addItem(new WayPoint(Point(id), id));

    pscene = scene;
}
