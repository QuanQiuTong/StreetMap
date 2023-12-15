#include <cmath>
#include <QWheelEvent>

#include "viewer.h"
#include "Items.h"

Viewer::Viewer(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(parent), m_scene(scene) { setScene(scene), setDragMode(ScrollHandDrag); /*RubberBandDrag*/ }

void Viewer::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double factor = std::pow(1.125, numDegrees / 15.0);
    scale(factor, factor);
}

void Viewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF p = mapToScene(event->pos());
    // extern long long nearestPoint(Point point);
    // auto id = nearestPoint(p);
    // printf("# click (%lf %lf), near [%lld](%lf %lf)\n",p.x(),p.y(),id,Point(id).x, Point(id).y);
    selectedPoints.push_back(new SelectPoint(p));
    m_scene->addItem(selectedPoints.back());
}

void Viewer::clearPoints()
{
    for (auto p : selectedPoints)
        m_scene->removeItem(p);
    selectedPoints.clear();
}

void Viewer::removeLastPoint()
{
    if (!selectedPoints.empty())
        m_scene->removeItem(selectedPoints.back()),
            selectedPoints.pop_back();
}

#include "path.h"
using path::SRC, path::DST;

Receiver receiver;
Receiver::Receiver() : shortPath(nullptr), findShortestPath(path::bidirectionalAStar), srcPos(), dstPos() {}
void Receiver::selectWayPoint(long long id)
{
    if (!SRC)
        SRC = id;
    else if (!DST)
        DST = id, findAndShow();
    else
        SRC = DST, srcPos = dstPos, DST = id, dstPos = Point(), findAndShow();
}
#include <set>
long long nearestPoint(Point point)
{
    extern std::set<long long> waypoints;
    return *min_element(waypoints.begin(), waypoints.end(), [point](auto a, auto b)
                        { return distance(point, a) < distance(point, b); });
}

void Receiver::selectRandomPoint(Point point)
{
    if (!SRC)
        SRC = nearestPoint(point), srcPos = point;
    else if (!DST)
        DST = nearestPoint(point), dstPos = point, findAndShow();
    else
        SRC = DST, srcPos = dstPos, DST = nearestPoint(point), dstPos = point, findAndShow();
}

void Receiver::clearPath()
{
    if (shortPath)
        pscene->removeItem(shortPath), shortPath = nullptr;
#if VISIBLE
    visible.clear();
#endif
    SRC = DST = 0;
    srcPos = dstPos = Point();
}

void Receiver::findAndShow()
{
    // printf("## SRC: %lld, DST: %lld\n", SRC, DST);
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
    // pscene->update();
}

void Receiver::dijkstra()
{
    findShortestPath = path::dijkstra;
    if (SRC && DST)
        findAndShow();
}

void Receiver::astar()
{
    findShortestPath = path::aStar;
    if (SRC && DST)
        findAndShow();
}

void Receiver::bidAstar()
{
    findShortestPath = path::bidirectionalAStar;
    if (SRC && DST)
        findAndShow();
}
