#include <cmath>
#include <QWheelEvent>

#include "viewer.h"

Viewer::Viewer(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(parent) { setScene(scene), setDragMode(ScrollHandDrag); /*RubberBandDrag*/ }

void Viewer::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = std::pow(1.125, numSteps);
    scale(factor, factor);
}
