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

class Building : public QGraphicsItem
{
public:
    Building(const QPolygonF &poly, const QString &text = "", QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_text(text),
          m_textColor(Qt::black), m_outlineColor(Qt::darkGray), m_backgroudColor(Qt::lightGray)
    {
        setFlags(ItemIsSelectable);

        m_polygon = poly;
        m_path.addPolygon(m_polygon);
    }
    ~Building() = default;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(widget);
        Q_UNUSED(option);
        painter->setPen(m_outlineColor);
        painter->setBrush(m_backgroudColor);
        painter->drawPolygon(m_polygon);
        painter->drawPath(m_path);
        painter->setPen(m_textColor);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
    }
    QRectF boundingRect() const override { return m_polygon.boundingRect(); }
    QPainterPath shape() const override { return m_path; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mousePressEvent(event);
        update();
    }

private:
    QPolygonF m_polygon;
    QPainterPath m_path;
    QString m_text;
    QColor m_textColor;
    QColor m_outlineColor;
    QColor m_backgroudColor;
};

class WayPoint : public QGraphicsItem
{
public:
    WayPoint(double x, double y, double radius, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_rect(x - radius, y - radius, 2 * radius, 2 * radius)
    {
        setFlags(ItemIsSelectable);
    }
    ~WayPoint() = default;
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
        printf("mousePress\n");
    }

private:
    QRectF m_rect;
};

#endif // ITEMS_H
