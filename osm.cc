#include <stdexcept>
#include <utility>

// Directly included for they cannot be compiled by Make correctly.
#include "tinyxml.cpp"
#include "tinyxmlparser.cpp"
#include "tinyxmlerror.cpp"
#define forSibling(p, t) for (; p && ((TiXmlNode *)p)->ValueTStr() == t; p = p->NextSiblingElement())
// static const char *att(TiXmlElement *p, const char *attr)
//{
//     const char *ret = p->Attribute(attr);
//     if (!ret)
//         throw std::invalid_argument("Attribute " + std::string(attr) + " not found");
//     return ret;
// }
#define att(p, attr) (p->Attribute(attr))
static double attd(TiXmlElement *p, const char *attr) { return atof(att(p, attr)); }
static long long id(TiXmlElement *p) { return atoll(att(p, "id")); }
static long long ref(TiXmlElement *p) { return atoll(att(p, "ref")); }

#include "osm.h"
#include "load.h"
NAMESPACE_OSM

double minlat, minlon, maxlat, maxlon;
AssocCon<long long, Node> nodes;

std::vector<Way> closedways, openways;
// AssocCon<long long, Way> ways;
// AssocCon<ll, Relation> relations;
bool parse(std::string filename)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(filename))
        return false;
    TiXmlElement *p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    minlat = attd(p, "minlat"), minlon = attd(p, "minlon"), maxlat = attd(p, "maxlat"), maxlon = attd(p, "maxlon");
    p = p->NextSiblingElement();
    nodes.clear();
    closedways.clear(), openways.clear();
    // relations.clear();
    forSibling(p, "node") nodes.insert({id(p), {attd(p, "lon"), attd(p, "lat")}});
    forSibling(p, "way")
    {
        Way w;
        TiXmlElement *q = p->FirstChildElement();
        forSibling(q, "nd") w.nd.push_back(ref(q));
        forSibling(q, "tag") w.tag.insert({att(q, "k"), att(q, "v")});

        if (w.nd.front() == w.nd.back())
            // ::loadClosedWay(w);
            closedways.push_back(std::move(w));
        else if (w.tag.count("highway"))
            // ::loadOpenWay(w);
            openways.push_back(std::move(w));
    }
    // forSibling(p, "relation")
    // {
    //     auto &r = relations[id(p)];
    //     TiXmlElement *q = p->FirstChildElement();
    //     forSibling(q, "member") r.member.push_back({att(q, "type"), ref(q), att(q, "role")});
    //     forSibling(q, "nd") r.tag.insert({att(q, "k"), att(q, "v")});
    // }
    return true;
}

END_NAMESPACE_OSM

using namespace osm;
#include <cmath>
#define FAST_DISTANCE 1
#if FAST_DISTANCE
double _dist(Node p, Node q)
{
    return hypot(p.lon - q.lon, p.lat - q.lat) * (M_PI * 6378137 / 180);
}
#else
static double rad(double deg) { return deg * M_PI / 180; };
double _dist(Node p, Node q)
{
    double dLat = rad(q.lat - p.lat);
    double dLon = rad(q.lon - p.lon);
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(rad(p.lat)) * cos(rad(q.lat)) *
                   sin(dLon / 2) * sin(dLon / 2);
    return 2 * 6378137 * asin(sqrt(a));
}
#endif

double realDist(long long u, long long v)
{
    return _dist(nodes.at(u), nodes.at(v));
}
