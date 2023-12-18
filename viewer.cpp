#include <cmath>
#include <QWheelEvent>

#include "viewer.h"
#include "path.h"
#include "Items.h"
using namespace path;

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
        scene()->removeItem(selectedPoints.back()),
            delete selectedPoints.back(),
            selectedPoints.pop_back();
}

void Viewer::removeLastPoint()
{
    if (!selectedPoints.empty())
        scene()->removeItem(selectedPoints.back()),
            delete selectedPoints.back(),
            selectedPoints.pop_back();
}

void Viewer::clearPath()
{
    if (shortPath)
        scene()->removeItem(shortPath), delete shortPath, shortPath = nullptr;
#if VISIBLE
    visible.clear();
#endif
    SRC = DST = 0;
    srcPos = dstPos = Point();
}
Path *shortPath;
std::vector<Point> (*Viewer::findShortestPath)(ll, ll) = bidirectionalAStar;

#include <QStatusBar>

void findAndShow()
{
    // printf("## SRC: %lld, DST: %lld\n", SRC, DST);
    if (!SRC || !DST)
        return;
    if (shortPath)
        scene->removeItem(shortPath), delete shortPath, shortPath = nullptr;
#if VISIBLE
    visible.clear();
#endif
    std::vector<Point> shortestPath = Viewer::findShortestPath(SRC, DST);
    double totalLength = totalDist;
    if (srcPos)
        shortestPath.insert(shortestPath.begin(), srcPos), totalLength += distance(srcPos, osm::nodes.at(SRC));
    if (dstPos)
        shortestPath.push_back(dstPos), totalLength += distance(dstPos, osm::nodes.at(DST));
    QString message = QString("Total Length: %1 metres").arg(totalLength);
#if VISIBLE
    message += QString(";  Selected Edges: %1;  Visible Edges: %2").arg(shortestPath.size() - 1).arg(visible.lines.size());
#endif
    // emit showStatusMessage(message);
    statusBar->showMessage(message);
    auto it = shortestPath.begin();
    QPainterPath painterPath{*it};
    while (++it != shortestPath.end())
        painterPath.lineTo(*it);
    scene->addItem(shortPath = new Path(painterPath));
}

#if VISIBLE
Visible visible;
#endif
