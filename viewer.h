#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsView>

class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QWidget* parent = 0);
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // VIEWER_H
