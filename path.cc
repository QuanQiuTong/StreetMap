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

static std::vector<ll> dijkstra(ll src, ll dst)
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
    std::vector<ll> ret;
    for (auto u = dst; u != src; u = prev[u])
        ret.push_back(u);
    ret.push_back(src);
    //std::reverse(ret.begin(), ret.end());
    
    // printf("Path:\n");
    // for(auto id: ret)
    //     printf("%lld ",id);
    // puts("");
    return ret;
}

END_NAMESPACE_PATH;
using namespace path;

Receiver receiver;
void Receiver::selectWayPoint(long long id)
{
    if (!SRC)
        SRC = id;
    else if (id != SRC && !DST)
        DST = id, receiver.findAndShow();
    else if (id != DST)
        SRC = DST, DST = id, receiver.findAndShow();
}
extern std::set<long long> waypoints;
long long nearestPoint(Point point)
{
    return *std::min_element(waypoints.begin(), waypoints.end(), [point](auto a, auto b)
                            { return distance(point, a) < distance(point, b); });
}
void Receiver::selectRandomPoint(Point point)
{
    if (!SRC)
        SRC = nearestPoint(point);
    else if (SRC != nearestPoint(point) && !DST)
        DST = nearestPoint(point), receiver.findAndShow();
    else if (DST != nearestPoint(point))
        SRC = DST, DST = nearestPoint(point), receiver.findAndShow();
    printf("SRC: %lld, DST: %lld\n", SRC, DST);
}

Path *Receiver::shortPath = nullptr;

void Receiver::clearPath()
{
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
    SRC = DST = 0;
}

void Receiver::findAndShow()
{
    printf("## SRC: %lld, DST: %lld\n", SRC, DST);
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
    std::vector<ll> path = dijkstra(SRC, DST);
    QPainterPath painterPath;
    auto it = path.begin();
    painterPath.moveTo(Point(*it));
    while (++it != path.end())
        painterPath.lineTo(Point(*it));
    pscene->addItem(shortPath = new Path(painterPath));
    // pscene->update();
}
