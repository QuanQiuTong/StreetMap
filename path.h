#ifndef PATH_H
#define PATH_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <QObject>

#include "osm.h"

#define NAMESPACE_PATH \
    namespace path     \
    {
#define END_NAMESPACE_PATH }

class QGraphicsScene;
extern QGraphicsScene *pscene;

NAMESPACE_PATH

using ll = long long;
template <typename Key, typename Val>
using AssocCon = std::unordered_map<Key, Val>;

extern AssocCon<ll, std::vector<ll>> path;
extern ll SRC, DST;

void addEdge(ll u, ll v);

void findAndShow();

END_NAMESPACE_PATH;

class Receiver : public QObject
{
public slots:
    void selectSource() { path::SRC = lastSelected; }
    void selectDestination() { path::DST = lastSelected; }
    void findAndShow();

public:
    path::ll lastSelected;
};
extern Receiver receiver;

#endif // PATH_H
