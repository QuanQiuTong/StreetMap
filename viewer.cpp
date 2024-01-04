#include <cmath>
#include <QWheelEvent>

#include "viewer.h"
#include "path.h"
#include "Items.h"

std::vector<SelectPoint*> selectedPoints;
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
std::vector<Point> (*findShortestPath)(ll, ll) = bidirectionalAStar;

#include <QStatusBar>
#include <chrono>

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
    using namespace std::chrono;
    high_resolution_clock::time_point startTime = high_resolution_clock::now();

    std::vector<Point> shortestPath = findShortestPath(SRC, DST);

    high_resolution_clock::time_point endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    
    double totalLength = totalDist;
    if (srcPos)
        shortestPath.insert(shortestPath.begin(), srcPos), totalLength += distance(srcPos, osm::nodes.at(SRC));
    if (dstPos)
        shortestPath.push_back(dstPos), totalLength += distance(dstPos, osm::nodes.at(DST));
    QString message = QString("Total Length: %1 metres; Time Elapsed: %2 microseconds").arg(totalLength).arg(duration);
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
