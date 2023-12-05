#ifndef OSM_H
#define OSM_H

#include <cstdlib>

#include <stdexcept>

#include <vector>
#include <map>
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

struct Bounds
{
    double minlat, minlon, maxlat, maxlon;
};

struct Node
{
    double lon, lat;
};
struct Way
{
    std::vector<Node> nd;
    std::map<std::string, std::string> tag;
};
struct Member
{
    std::string type;
    ll ref;
    std::string role;
    Member() = default;
    Member(std::string _type, ll _ref, std::string _role) : type(_type), ref(_ref), role(_role) {}
};
struct Relation
{
    std::vector<Member> member;
    std::map<std::string, std::string> tag;
};

template <typename Metadata>
struct element : public Metadata
{
    ll id;
};

template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

extern AssocCon<ll, Node> nodes;
extern AssocCon<ll, Way> ways;
extern AssocCon<ll, Relation> relations;

Bounds parse(FILE *infile);

// struct node
// {
//     ll id;
//     double lon, lat;
// };
// struct way
// {
//     ll id;
//     std::vector<node> nodes;
//     std::map<std::string, std::string> tags;
// };

END_NAMESPACE_OSM

#endif
