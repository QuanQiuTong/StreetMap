#ifndef OSM_H
#define OSM_H

#include <cstdlib>

#include <stdexcept>

#include <vector>
#include <map>
#include <string>

#define TIXML_USE_STL
#include "tinyxml.h"

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
    std::string type, role;
    ll ref;
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

template <template <typename Key, typename Val> typename AssocCon>
Bounds parse(FILE *infile, AssocCon<ll, Node> &nodes, AssocCon<ll, Way> &ways, AssocCon<ll, Relation> &relations);

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
