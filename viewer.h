#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <QGraphicsView>
#include "point.h"
class SelectPoint;

class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QGraphicsScene *scene, QWidget *parent = nullptr);
public slots:
    void clearPoints();
    void removeLastPoint();
    void clearPath();
    void dijkstra();
    void astar();
    void bidAstar();

protected:
    void wheelEvent(QWheelEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    std::vector<SelectPoint *> selectedPoints;
};
void findAndShow();

#endif // VIEWER_H
