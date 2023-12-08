#ifndef ITEMS_H
#define ITEMS_H

#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
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
    Building(const QPolygonF &poly, const QString &text = "", QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_polygon(poly), m_text(text) { /*setFlags(ItemIsSelectable),*/ m_path.addPolygon(m_polygon); }

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

class WayPoint : public QGraphicsItem
{
public:
    WayPoint(double x, double y, double radius = 5, long long id = 0, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_rect(x - radius, y - radius, 2 * radius, 2 * radius), m_id(id) { setFlags(ItemIsSelectable); }
    WayPoint(QPointF p, long long id, double radius = 5, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_rect(p.x() - radius, p.y() - radius, 2 * radius, 2 * radius), m_id(id) { setFlags(ItemIsSelectable); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(widget);
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::white);
        painter->drawEllipse(m_rect);
    }
    QRectF boundingRect() const override { return m_rect; }
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addEllipse(m_rect);
        return path;
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mousePressEvent(event);
        update();
        if (!path::SRC)
            path::SRC = m_id;
        else if (m_id != path::SRC && !path::DST)
            path::DST = m_id, receiver.findAndShow();
        else if (m_id != path::DST)
            path::SRC = path::DST, path::DST = m_id, receiver.findAndShow();
    }

private:
    QRectF m_rect;
    long long m_id;
};

class Path : public QGraphicsItem
{
    QPainterPath m_path;

public:
    double width;
    Path(const QPainterPath &path, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), m_path(path), width(2) {}
    ~Path()
    {
        // prepareGeometryChange();
        // printf("Path Removed (automatically)\n");
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(option) Q_UNUSED(widget)
        QPen pen(Qt::red, 4);
        pen.setWidthF(pen.widthF() / painter->transform().m11());
        painter->setPen(pen);
        painter->drawPath(m_path);
    }
    QRectF boundingRect() const override { return m_path.boundingRect(); }
    QPainterPath shape() const override { return m_path; }
};
extern Path *shortPath;

#endif // ITEMS_H
