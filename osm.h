#ifndef OSM_H
#define OSM_H

#include <vector>
// #include <map>
#include <unordered_map>
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/hash_policy.hpp>
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
extern std::vector<Way> closedways, openways;
// extern AssocCon<long long, Way> ways;
// extern AssocCon<long long, Relation> relations;

bool parse(std::string filename);

END_NAMESPACE_OSM
double _dist(osm::Node,osm::Node);
double realDist(long long u, long long v);
#endif
