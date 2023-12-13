#include <cmath>
#include <QWheelEvent>

#include "viewer.h"
#include "Items.h"

Viewer::Viewer(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(parent), m_scene(scene) { setScene(scene), setDragMode(ScrollHandDrag); /*RubberBandDrag*/ }

void Viewer::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = std::pow(1.125, numSteps);
    scale(factor, factor);
}
extern long long nearestPoint(Point point);
void Viewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF p = mapToScene(event->pos());
    auto id = nearestPoint(p);
    printf("# click (%lf %lf), near [%lld](%lf %lf)\n",p.x(),p.y(),id,Point(id).x, Point(id).y);
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
    if (selectedPoints.empty())
        return;
    m_scene->removeItem(selectedPoints.back());
    selectedPoints.pop_back();
}
