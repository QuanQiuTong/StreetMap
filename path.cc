#include "path.h"

#include <cmath>
#include <queue>
#include <set>
#include <algorithm>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "Items.h"
#include "point.h"

QGraphicsScene *pscene;

NAMESPACE_PATH

using namespace std; // limited to this file

AssocCon<ll, vector<ll>> path;
ll SRC, DST;
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
            if (double alt = dist[u] + distance(Point(u), Point(v)); !dist.count(v) || alt < dist[v])
#if VISIBLE
                visible.addEdge(u, v),
#endif
                    dist[v] = alt,
                    prev[v] = u,
                    q.push({v, alt});
    }
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
            if (double alt = dist[u] + distance(Point(u), Point(v)); !dist.count(v) || alt < dist[v])
#if VISIBLE
                visible.addEdge(u, v),
#endif
                    dist[v] = alt,
                    prev[v] = u,
                    q.push({v, alt + distance(Point(v), Point(dst))});
    }
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
            if (double alt = distForward[uForward] + distance(Point(uForward), Point(vForward));
                !distForward.count(vForward) || alt < distForward[vForward])
#if VISIBLE
                visible.addEdge(uForward, vForward),
#endif
                    distForward[vForward] = alt,
                    prevForward[vForward] = uForward,
                    qForward.push({vForward, alt + distance(Point(vForward), Point(dst))});

        ll uBackward = qBackward.top().id;
        qBackward.pop();
        if (prevForward.count(uBackward))
        {
            meetingNode = uBackward;
            break;
        }
        for (ll vBackward : path.at(uBackward))
            if (double alt = distBackward[uBackward] + distance(Point(uBackward), Point(vBackward));
                !distBackward.count(vBackward) || alt < distBackward[vBackward])
#if VISIBLE
                visible.addEdge(uBackward, vBackward),
#endif
                    distBackward[vBackward] = alt,
                    prevBackward[vBackward] = uBackward,
                    qBackward.push({vBackward, alt + distance(Point(vBackward), Point(src))});
    }
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

#if VISIBLE
Visible visible;
#endif
