#ifndef ITEMS_H
#define ITEMS_H

#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "path.h"

static const QColor textColor = Qt::black, outlineColor = Qt::darkGray, backgroundColor = Qt::lightGray;
class Building : public QGraphicsItem
{
    QPolygonF m_polygon;
    QPainterPath m_path;
    QString m_text;

public:
    Building(QPolygonF poly, const QString &text = "", QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_polygon(poly), m_text(text)
    { /*setFlags(ItemIsSelectable),*/
        m_path.addPolygon(m_polygon);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setPen(outlineColor);
        painter->setBrush(backgroundColor);
        painter->drawPolygon(m_polygon);
        painter->drawPath(m_path);
        painter->setPen(textColor);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
    }
    QRectF boundingRect() const override { return m_polygon.boundingRect(); }
    QPainterPath shape() const override { return m_path; }

    // protected:
    //     void mousePressEvent(QGraphicsSceneMouseEvent *event)
    //         override { QGraphicsItem::mousePressEvent(event), update(); }
};

#include <QGraphicsEllipseItem>

class WayPoint : public QGraphicsEllipseItem
{
    long long m_id;

public:
    WayPoint(double x, double y, double radius = 5, long long id = 0, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(x - radius, y - radius, 2 * radius, 2 * radius, parent), m_id(id) { setFlags(ItemIsSelectable); }
    WayPoint(QPointF p, long long id, double radius = 5, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(p.x() - radius, p.y() - radius, 2 * radius, 2 * radius, parent), m_id(id) { setFlags(ItemIsSelectable); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::white);
        painter->drawEllipse(rect());
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override { receiver.selectWayPoint(m_id); }
};

class Path : public QGraphicsItem
{
    QPainterPath m_path;

public:
    Path(QPainterPath path, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), m_path(path) {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        QPen pen(Qt::red, 4);
        pen.setWidthF(pen.widthF() / painter->transform().m11());
        painter->setPen(pen);
        painter->drawPath(m_path);
    }
    QRectF boundingRect() const override { return m_path.boundingRect(); }
    QPainterPath shape() const override { return m_path; }
};

class SelectPoint : public QGraphicsItem
{
    static size_t cnt;
    QPointF point;
    double radius;

public:
    SelectPoint(QPointF p, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), point(p), radius(6.0) {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        radius = 6.0 / painter->transform().m11();
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::red);
        painter->drawEllipse(point, radius, radius);
    }
    QRectF boundingRect() const override { return {point.x() - radius, point.y() - radius, 2 * radius, 2 * radius}; }
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *) override { receiver.selectRandomPoint(point); }
};

#endif // ITEMS_H
