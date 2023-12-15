#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <QGraphicsView>
class QGraphicsScene;
class SelectPoint;

class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QGraphicsScene *scene, QWidget *parent = nullptr);
public slots:
    void clearPoints();
    void removeLastPoint();

protected:
    void wheelEvent(QWheelEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *m_scene;
    std::vector<SelectPoint *> selectedPoints;
};

#include "point.h"
class Path;
class Receiver : public QObject
{
    Q_OBJECT
public slots:
    void findAndShow();
    void clearPath();
    void dijkstra();
    void astar();
    void bidAstar();

private:
    Path *shortPath;
    std::vector<Point> (*findShortestPath)(ll, ll);

public:
    Point srcPos, dstPos;
    Receiver();
    void selectWayPoint(long long point);
    void selectRandomPoint(Point point);
};
extern Receiver receiver;

#endif // VIEWER_H
