#ifndef ITEMS_H
#define ITEMS_H

#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include "viewer.h"
#include "path.h"
using path::SRC, path::DST, path::srcPos, path::dstPos;

static const QColor textColor = Qt::black, outlineColor = Qt::darkGray, backgroundColor = Qt::lightGray;
class Building : public QGraphicsPolygonItem
{
    QString m_text;

public:
    Building(QPolygonF poly, const QString &text = "", QGraphicsItem *parent = nullptr)
        : QGraphicsPolygonItem(poly, parent), m_text(text) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setPen(outlineColor);
        painter->setBrush(backgroundColor);
        painter->drawPolygon(polygon());
        painter->setPen(textColor);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
    }
};

class WayPoint : public QGraphicsEllipseItem
{
    long long id;

public:
    WayPoint(Point p, long long _id, double radius = 5, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(p.x - radius, p.y - radius, 2 * radius, 2 * radius, parent), id(_id) { setFlags(ItemIsSelectable); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::white);
        painter->drawEllipse(rect());
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override
    {
        if (!SRC)
            SRC = id;
        else if (!DST)
            DST = id, findAndShow();
        else
            SRC = DST, srcPos = dstPos, DST = id, dstPos = Point(), findAndShow();
    }
};

struct Path : public QGraphicsPathItem
{
    Path(QPainterPath path, QGraphicsItem *parent = nullptr) : QGraphicsPathItem(path, parent) {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setPen({Qt::red, 4 / std::min(painter->transform().m11(), 1.0)});
        painter->drawPath(path());
    }
};
extern Path *shortPath;

class SelectPoint : public QGraphicsItem
{
    Point point;
    double radius;

public:
    SelectPoint(QPointF p, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), point(p), radius(6.0) {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        radius = 6.0 / std::min(painter->transform().m11(), 1.0);
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::red);
        painter->drawEllipse(point, radius, radius);
    }
    QRectF boundingRect() const override { return {point.x - radius, point.y - radius, 2 * radius, 2 * radius}; }
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *) override
    {
        if (!SRC)
            SRC = nearestPoint(point), srcPos = point;
        else if (!DST)
            DST = nearestPoint(point), dstPos = point, findAndShow();
        else
            SRC = DST, srcPos = dstPos, DST = nearestPoint(point), dstPos = point, findAndShow();
    }
};

#endif // ITEMS_H
