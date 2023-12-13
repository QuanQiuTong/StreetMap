#ifndef PATH_H
#define PATH_H

#include <cmath>
#include <vector>
#include <unordered_map>
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

using ll = long long;
template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

extern AssocCon<ll, std::vector<ll>> path;
extern ll SRC, DST;

void addEdge(ll u, ll v);

END_NAMESPACE_PATH;

class Receiver : public QObject
{
public slots:
    void selectSource() { path::SRC = lastSelected; }
    void selectDestination() { path::DST = lastSelected; }
    void findAndShow();
    void clearPath();

private:
    // struct SourceDestination
    // {
    //     bool onWay;
    //     union
    //     {
    //         Point p;
    //         long long id;
    //     } point;
    // } src, dst;

    static Path *shortPath;

public:
    Point srcPos, dstPos;
    path::ll lastSelected;
    void selectWayPoint(path::ll point);
    void selectRandomPoint(Point point);
};
extern Receiver receiver;

#endif // PATH_H
