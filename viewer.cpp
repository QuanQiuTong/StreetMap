#include <cmath>
#include <QWheelEvent>

#include "viewer.h"
#include "Items.h"

Viewer::Viewer(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) { setDragMode(ScrollHandDrag); }

void Viewer::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double factor = std::pow(1.125, numDegrees / 15.0);
    scale(factor, factor);
}

void Viewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    selectedPoints.push_back(new SelectPoint(mapToScene(event->pos())));
    scene()->addItem(selectedPoints.back());
}

void Viewer::clearPoints()
{
    while (!selectedPoints.empty())
        scene()->removeItem(selectedPoints.back()), selectedPoints.pop_back();
}

void Viewer::removeLastPoint()
{
    if (!selectedPoints.empty())
        scene()->removeItem(selectedPoints.back()), selectedPoints.pop_back();
}

void Viewer::clearPath()
{
    if (shortPath)
        scene()->removeItem(shortPath), shortPath = nullptr;
#if VISIBLE
    visible.clear();
#endif
    SRC = DST = 0;
    srcPos = dstPos = Point();
}

#include "path.h"
Path* shortPath;
static std::vector<Point> (*findShortestPath)(ll, ll);
void findAndShow()
{
    using namespace path;
    // printf("## SRC: %lld, DST: %lld\n", SRC, DST);
    if (!SRC || !DST) return;
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
#if VISIBLE
    visible.clear();
#endif
    std::vector<Point> shortestPath = findShortestPath(SRC, DST);
#if VISIBLE
    visible.show();
#endif
    if (srcPos)
        shortestPath.insert(shortestPath.begin(), srcPos);
    if (dstPos)
        shortestPath.push_back(dstPos);
    QPainterPath painterPath;
    auto it = shortestPath.begin();
    painterPath.moveTo(*it);
    while (++it != shortestPath.end())
        painterPath.lineTo(*it);
    pscene->addItem(shortPath = new Path(painterPath));
}

void Viewer::dijkstra()
{
    findShortestPath = path::dijkstra;
        findAndShow();
}

void Viewer::astar()
{
    findShortestPath = path::aStar;
        findAndShow();
}

void Viewer::bidAstar()
{
    findShortestPath = path::bidirectionalAStar;
        findAndShow();
}
