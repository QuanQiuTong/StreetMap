#ifndef PATH_H
#define PATH_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <QObject>

#include "osm.h"
#include "point.h"

#define NAMESPACE_PATH \
    namespace path     \
    {
#define END_NAMESPACE_PATH }

class QGraphicsScene;
class Path;
extern QGraphicsScene *pscene;

NAMESPACE_PATH

template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

extern AssocCon<ll, std::vector<ll>> path;
extern ll SRC, DST;
extern Point srcPos, dstPos;

void addEdge(ll u, ll v);

std::vector<Point> dijkstra(ll src, ll dst);
std::vector<Point> aStar(ll src, ll dst);
std::vector<Point> bidirectionalAStar(ll src, ll dst);

END_NAMESPACE_PATH;

#define VISIBLE 1

#if VISIBLE

#include <QPainter>
#include <QGraphicsLineItem>
#include <QGraphicsScene>

class VisibleLine : public QGraphicsLineItem
{
public:
    VisibleLine(ll u, ll v, QGraphicsItem *parent = nullptr) : QGraphicsLineItem(Point(u).x, Point(u).y, Point(v).x, Point(v).y, parent) {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setPen({Qt::yellow, 3.5 / std::min(painter->transform().m11(), 1.0)});
        painter->drawLine(line());
    }
};
class Visible
{
    std::vector<std::pair<ll, ll>> edgesVisited;
    std::vector<VisibleLine *> lines;

public:
    void addEdge(ll u, ll v) { edgesVisited.push_back({u, v}); }
    void show()
    {
        for (auto [u, v] : edgesVisited)
            lines.push_back(new VisibleLine(u, v)),
                pscene->addItem(lines.back());
    }
    void clear()
    {
        for (auto line : lines)
            pscene->removeItem(line);
        lines.clear();
        edgesVisited.clear();
    }
};
extern Visible visible;
#endif

#endif // PATH_H
