#ifndef OSM_H
#define OSM_H

#include <cmath>
#include <vector>
// #include <map>
#include <unordered_map>
#include <string>

#define NAMESPACE_OSM \
    namespace osm     \
    {
#define END_NAMESPACE_OSM \
    }                     \
    ;

NAMESPACE_OSM

template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

struct Node
{
    double lon, lat;
    friend double distance(Node p, Node q)
    {
        auto rad = [](double deg) { return deg * M_PI / 180; };
        double dLat = rad(q.lat - p.lat);
        double dLon = rad(q.lon - p.lon);
        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(rad(p.lat)) * cos(rad(q.lat)) *
                       sin(dLon / 2) * sin(dLon / 2);
        return 2 * 6378137 * asin(sqrt(a));
    }
};
struct Way
{
    std::vector<long long> nd;
    AssocCon<std::string, std::string> tag;
};
// struct Member
// {
//     std::string type;
//     long long ref;
//     std::string role;
// };
// struct Relation
// {
//     std::vector<Member> member;
//     AssocCon<std::string, std::string> tag;
// };

extern double minlat, minlon, maxlat, maxlon;
extern AssocCon<long long, Node> nodes;
extern AssocCon<long long, Way> closedways, openways;
// extern AssocCon<long long, Relation> relations;

void parse(FILE *);

END_NAMESPACE_OSM

double realDist(long long, long long);
#endif
