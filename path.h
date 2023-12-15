#ifndef PATH_H
#define PATH_H

#include <vector>
#include <unordered_map>

#include "point.h"

#define NAMESPACE_PATH \
    namespace path     \
    {
#define END_NAMESPACE_PATH }

NAMESPACE_PATH

template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

extern AssocCon<ll, std::vector<ll>> path;
extern ll SRC, DST;
extern Point srcPos, dstPos;
extern double totalDist;

void addEdge(ll u, ll v);

std::vector<Point> dijkstra(ll src, ll dst);
std::vector<Point> aStar(ll src, ll dst);
std::vector<Point> bidirectionalAStar(ll src, ll dst);

END_NAMESPACE_PATH;

#endif // PATH_H
