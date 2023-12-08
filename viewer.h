#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsView>
//#include <cstdio>
class QGraphicsScene;

class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QGraphicsScene *scene, QWidget *parent = nullptr);
    //~Viewer(){puts("Auto del");}

protected:
    void wheelEvent(QWheelEvent *);
};

extern double viewerFactor;

#endif // VIEWER_H
