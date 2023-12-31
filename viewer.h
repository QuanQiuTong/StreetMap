#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include "point.h"
#include "path.h"
#include "streetmap.h"
class SelectPoint;
void findAndShow();

extern std::vector<SelectPoint*> selectedPoints;
extern std::vector<Point> (*findShortestPath)(ll, ll);

class Viewer : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer(QGraphicsScene *scene, QWidget *parent = nullptr);
public slots:
    void clearPath();

protected:
    void wheelEvent(QWheelEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#define VISIBLE 1

#if VISIBLE

#include <QPainter>
#include <QGraphicsScene>
extern QGraphicsScene* scene;
static constexpr QColor lightBlue(173,216,230);
struct Visible
{
     class VisibleLine : public QGraphicsLineItem
     {

     public:
         VisibleLine(long long u, long long v, QGraphicsItem *parent = nullptr)
             : QGraphicsLineItem(Point(u).x, Point(u).y, Point(v).x, Point(v).y, parent) {}
         void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
         {
             painter->setPen({QColor(255, 160, 100), 4 / std::min(painter->transform().m11(), 1.0)});
             painter->drawLine(line());
         }
     };
     std::vector<VisibleLine*> lines;
     void addLine(long long u, long long v)
     {
         lines.push_back(new VisibleLine(u, v));
         scene->addItem(lines.back());
     }

//    std::vector<QGraphicsLineItem*> lines;
//    void addLine(long long u,long long v){
//        lines.push_back(scene->addLine(Point(u).x, Point(u).y, Point(v).x, Point(v).y, QPen(lightBlue, 100)));
//    }

    void clear()
    {
        while(!lines.empty())
            scene->removeItem(lines.back()), delete lines.back(), lines.pop_back();
    }
};
extern Visible visible;
#endif

#endif // VIEWER_H
