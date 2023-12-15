#include <stdexcept>
#include <utility>

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
AssocCon<long long, Node> nodes;
std::vector<Way> closedways, openways;
// AssocCon<long long, Way> ways;
// AssocCon<ll, Relation> relations;
void parse(FILE *fp)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(fp))
        throw std::invalid_argument("File load failed");
    TiXmlElement *p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    minlat = attd(p, "minlat"), minlon = attd(p, "minlon"), maxlat = attd(p, "maxlat"), maxlon = attd(p, "maxlon");
    p = p->NextSiblingElement();
    nodes.clear(), closedways.clear(), openways.clear(); // relations.clear();
    forSibling(p, "node") nodes.insert({id(p), {attd(p, "lon"), attd(p, "lat")}});
    forSibling(p, "way")
    {
        Way w;
        TiXmlElement *q = p->FirstChildElement();
        forSibling(q, "nd") w.nd.push_back(ref(q));
        forSibling(q, "tag") w.tag.insert({att(q, "k"), att(q, "v")});
        (w.nd.front() == w.nd.back() ? closedways : openways).push_back(std::move(w));
    }
    // forSibling(p, "relation")
    // {
    //     auto &r = relations[id(p)];
    //     TiXmlElement *q = p->FirstChildElement();
    //     forSibling(q, "member") r.member.push_back({att(q, "type"), ref(q), att(q, "role")});
    //     forSibling(q, "nd") r.tag.insert({att(q, "k"), att(q, "v")});
    // }
}

END_NAMESPACE_OSM

double realDist(long long u, long long v)
{
    return distance(osm::nodes.at(u), osm::nodes.at(v));
}
