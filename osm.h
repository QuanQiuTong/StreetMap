#ifndef OSM_H
#define OSM_H

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

using ll = long long;

template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

struct Node
{
    double lon, lat;
};
struct Way
{
    std::vector<ll> nd;
    AssocCon<std::string, std::string> tag;
};
struct Member
{
    std::string type;
    ll ref;
    std::string role;
};
struct Relation
{
    std::vector<Member> member;
    AssocCon<std::string, std::string> tag;
};

// template <typename Metadata>
// struct element : public Metadata
// {
//     ll id;
// };

extern double minlat, minlon, maxlat, maxlon;
extern AssocCon<ll, Node> nodes;
extern AssocCon<ll, Way> ways;
// extern AssocCon<ll, Relation> relations;

END_NAMESPACE_OSM

#endif
