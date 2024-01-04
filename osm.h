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
    static double rad(double deg) { return deg * M_PI / 180; }
#define FAST_DISTANCE 1
#if FAST_DISTANCE

    /**
     * @brief Fast distance calculation.
     * 
     * This function calculates the distance between two nodes using a fast approximation.
     * 
     * @param p The first node.
     * @param q The second node.
     * @return The approximate distance between the two nodes in meters.
     */
    friend double distance(Node p, Node q)
    {
        return hypot((p.lon - q.lon) * 0.8544, p.lat - q.lat) * (M_PI * 6378137 / 180);
    }
#else
    /**
     * Calculates the distance between two nodes using the Haversine formula.
     * 
     * @param p The first node.
     * @param q The second node.
     * @return The distance between the two nodes in meters.
     */
    friend double distance(Node p, Node q)
    {
        double dLat = rad(q.lat - p.lat);
        double dLon = rad(q.lon - p.lon);
        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(rad(p.lat)) * cos(rad(q.lat)) *
                       sin(dLon / 2) * sin(dLon / 2);
        return 2 * 6378137 * asin(sqrt(a));
    }
#endif
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
//  extern AssocCon<long long, Relation> relations;

void parse(FILE *);

END_NAMESPACE_OSM

double realDist(long long, long long);
#endif
