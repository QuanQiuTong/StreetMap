#include "path.h"

#include <cmath>
#include <queue>
#include <set>
#include <algorithm>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QPointF>

#include "Items.h"
#include "point.h"

QGraphicsScene *pscene;

NAMESPACE_PATH

AssocCon<ll, std::vector<ll>> path;
ll SRC, DST;
void addEdge(ll u, ll v) { path[u].push_back(v), path[v].push_back(u); }

static std::vector<Point> dijkstra(ll src, ll dst)
{
    struct Node
    {
        ll id;
        double dist;
        bool operator<(const Node &other) const { return dist > other.dist; }
    };
    std::priority_queue<Node> q;
    q.push({src, 0});
    AssocCon<ll, ll> prev;
    AssocCon<ll, double> dist({{src, 0}});
    while (!q.empty())
    {
        auto [u, disu] = q.top();
        q.pop();
        if (u == dst)
            break;
        if (disu > dist[u])
            continue;
        if (!path.count(u))
            throw;
        for (ll v : path[u])
            if (double alt = dist[u] + distance(Point(u), Point(v)); !dist.count(v) || alt < dist[v])
                dist[v] = alt,
                prev[v] = u,
                q.push({v, alt});
    }
    std::vector<Point> ret;
    for (auto u = dst; u != src; u = prev[u])
        ret.push_back(Point(u));
    ret.push_back(Point(src));
    std::reverse(ret.begin(), ret.end());
    return ret;
}

END_NAMESPACE_PATH;
using namespace path;

Receiver receiver;
void Receiver::selectWayPoint(long long id)
{ // printf("0@ SRC: %lld, DST: %lld\n", SRC, DST);
    if (!SRC)
        // puts("*1"),
        SRC = id;
    else if (!DST)
        // puts("*2"),
        DST = id, findAndShow();
    else
        // puts("*3"),
        SRC = DST, srcPos = dstPos, DST = id, dstPos = Point(), findAndShow();
    // printf("1@ SRC: %lld, DST: %lld\n", SRC, DST);
}
extern std::set<long long> waypoints;
long long nearestPoint(Point point)
{
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](auto a, auto b)
                             { return distance(point, a) < distance(point, b); });
}
void Receiver::selectRandomPoint(Point point)
{ // printf("2@ SRC: %lld, DST: %lld\n", SRC, DST);
    if (!SRC)
        // puts("*4"),
        SRC = nearestPoint(point), srcPos = point;
    else if (!DST)
        // puts("*5"),
        DST = nearestPoint(point), dstPos = point, findAndShow();
    else
        // puts("*6"),
        SRC = DST, srcPos = dstPos, DST = nearestPoint(point), dstPos = point, findAndShow();
    // printf("3@ SRC: %lld, DST: %lld\n", SRC, DST);
}

Path *Receiver::shortPath = nullptr;

void Receiver::clearPath()
{
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
    SRC = DST = 0;
    srcPos = dstPos = Point();
}

void Receiver::findAndShow()
{
    printf("## SRC: %lld, DST: %lld\n", SRC, DST);
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
    QPainterPath painterPath;
    std::vector<Point> path = dijkstra(SRC, DST);

    if (srcPos)
        path.insert(path.begin(), srcPos);
    if (dstPos)
        path.push_back(dstPos);

    auto it = path.begin();
    painterPath.moveTo(*it);
    while (++it != path.end())
        painterPath.lineTo(*it);
    pscene->addItem(shortPath = new Path(painterPath));
    // pscene->update();
}
