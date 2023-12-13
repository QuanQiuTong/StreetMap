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
    std::vector<SelectPoint*> selectedPoints;
};

#endif // VIEWER_H
