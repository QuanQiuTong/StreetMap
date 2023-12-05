#include <cmath>
#include <QWheelEvent>

#include "viewer.h"

Viewer::Viewer(QWidget *parent)
    : QGraphicsView(parent)
{
    setDragMode(ScrollHandDrag); // RubberBandDrag
}

void Viewer::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = std::pow(1.125, numSteps);
    scale(factor, factor);
}
