/**
 * @file osm.cc
 * @brief Implementation of the osm namespace functions.
 */

//#include <stdexcept>

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
static inline double attd(TiXmlElement *p, const char *attr) { return atof(att(p, attr)); }
static inline long long id(TiXmlElement *p) { return atoll(att(p, "id")); }
static inline long long ref(TiXmlElement *p) { return atoll(att(p, "ref")); }

#include "osm.h"

NAMESPACE_OSM

double minlat, minlon, maxlat, maxlon;
AssocCon<long long, Node> nodes;
std::vector<Way> closedways, openways;
// AssocCon<long long, Way> ways;
// AssocCon<ll, Relation> relations;

/**
 * @brief Parse the OpenStreetMap XML file.
 * 
 * This function parses the OpenStreetMap XML file and populates the necessary data structures.
 * 
 * @param fp The file pointer to the OpenStreetMap XML file.
 * @throws std::invalid_argument if the file load fails.
 */
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
        if (w.nd.front() == w.nd.back()) // ::loadClosedWay(w);
            closedways.push_back(static_cast<Way&&>(w));
        else if (w.tag.count("highway")) // ::loadOpenWay(w);
            openways.push_back(static_cast<Way&&>(w));
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

/**
 * @brief Calculate the real distance between two nodes.
 * 
 * This is a wrapper function for the distance function in the osm namespace.
 * 
 * @param u The ID of the first node.
 * @param v The ID of the second node.
 * @return The real distance between the two nodes.
 */
double realDist(long long u, long long v)
{
    return distance(osm::nodes.at(u), osm::nodes.at(v));
}
