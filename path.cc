#include "path.h"

#include <queue>
#include <algorithm>

#include "point.h"
#include "osm.h"

#include "viewer.h"
#if VISIBLE
#define visitEdge(u, v) visible.addEdge(u, v),
#else
#define visitEdge(u, v)
#endif

NAMESPACE_PATH

using namespace std; // limited to this file

AssocCon<ll, vector<ll>> path;
ll SRC, DST;
Point srcPos, dstPos;
double totalDist;

void addEdge(ll u, ll v) { path[u].push_back(v), path[v].push_back(u); }
struct Node
{
    ll id;
    double dist;
    bool operator<(const Node &rhs) const { return dist > rhs.dist; }
};
vector<Point> dijkstra(ll src, ll dst)
{
    AssocCon<ll, ll> prev;
    AssocCon<ll, double> dist({{src, 0}});
    for (priority_queue<Node> q(less<Node>(), {{src, 0}}); !q.empty();)
    {
        auto u = q.top().id;
        q.pop();
        if (u == dst)
            break;
        for (ll v : path.at(u))
            if (double alt = dist[u] + realDist(u, v); !dist.count(v) || alt < dist[v])
                visitEdge(u, v)
                    dist[v] = alt,
                    prev[v] = u,
                    q.push({v, alt});
    }
    totalDist = dist[dst];
    vector<Point> ret;
    for (auto u = dst; u != src; u = prev[u])
        ret.push_back(Point(u));
    ret.push_back(Point(src));
    reverse(ret.begin(), ret.end());
    return ret;
}

vector<Point> aStar(ll src, ll dst)
{
    AssocCon<ll, ll> prev;
    AssocCon<ll, double> dist({{src, 0}});
    for (priority_queue<Node> q(less<Node>(), {{src, 0}}); !q.empty();)
    {
        auto u = q.top().id;
        q.pop();
        if (u == dst)
            break;
        for (ll v : path.at(u))
            if (double alt = dist[u] + realDist(u, v); !dist.count(v) || alt < dist[v])
                visitEdge(u, v)
                    dist[v] = alt,
                    prev[v] = u,
                    q.push({v, alt + realDist(v, dst)});
    }
    totalDist = dist[dst];
    vector<Point> ret;
    for (auto u = dst; u != src; u = prev[u])
        ret.push_back(Point(u));
    ret.push_back(Point(src));
    reverse(ret.begin(), ret.end());
    return ret;
}

vector<Point> bidirectionalAStar(ll src, ll dst)
{
    priority_queue<Node> qForward(less<Node>(), {{src, 0}}), qBackward(less<Node>(), {{dst, 0}});
    AssocCon<ll, ll> prevForward({{src, src}}), prevBackward({{dst, dst}});
    AssocCon<ll, double> distForward({{src, 0}}), distBackward({{dst, 0}});
    ll meetingNode = -1;

    while (!qForward.empty() && !qBackward.empty())
    {
        ll uForward = qForward.top().id;
        qForward.pop();
        if (prevBackward.count(uForward))
        {
            meetingNode = uForward;
            break;
        }
        for (ll vForward : path.at(uForward))
            if (double alt = distForward[uForward] + realDist(uForward, vForward);
                !distForward.count(vForward) || alt < distForward[vForward])
                visitEdge(uForward, vForward)
                    distForward[vForward] = alt,
                    prevForward[vForward] = uForward,
                    qForward.push({vForward, alt + realDist(vForward, dst)});

        ll uBackward = qBackward.top().id;
        qBackward.pop();
        if (prevForward.count(uBackward))
        {
            meetingNode = uBackward;
            break;
        }
        for (ll vBackward : path.at(uBackward))
            if (double alt = distBackward[uBackward] + realDist(uBackward, vBackward);
                !distBackward.count(vBackward) || alt < distBackward[vBackward])
                visitEdge(uBackward, vBackward)
                    distBackward[vBackward] = alt,
                    prevBackward[vBackward] = uBackward,
                    qBackward.push({vBackward, alt + realDist(vBackward, src)});
    }
    totalDist = distForward[meetingNode] + distBackward[meetingNode];
    vector<Point> ret;
    for (auto u = meetingNode; u != src;)
        ret.push_back(Point(u = prevForward[u]));
    reverse(ret.begin(), ret.end());
    for (auto u = meetingNode; u != dst; u = prevBackward[u])
        ret.push_back(Point(u));
    ret.push_back(Point(dst));
    return ret;
}

END_NAMESPACE_PATH;
