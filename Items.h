#ifndef ITEMS_H
#define ITEMS_H

#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QPainterPath>
#include <QPainter>

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

    void setText(const QString &text)
    {
        m_text = text;
        update();
    }
    QString text() const { return m_text; }
    void setTextColor(const QColor &color) { m_textColor = color, update(); }
    QColor textColor() const { return m_textColor; }
    //    void setOutlineColor(const QColor &color) { m_group->setPen(color), update(); }
    //    QColor outlineColor() const { return m_group->pen().color(); }
    //    void setBackgroudColor(const QColor &color) { m_group->setBrush(color), update(); }
    //    QColor backgroudColor() const { return m_group->brush().color(); }
    //    void setFillColor(const QColor &color) { m_group->setBrush(color), update(); }
    //    QColor fillColor() const { return m_group->brush().color(); }
    void setPolygon(const QPolygonF &polygon)
    {
        m_polygon = polygon;
        update();
    }
    QPolygonF polygon() const { return m_polygon; }
    void setPath(const QPainterPath &path) { m_path = path, update(); }
    QPainterPath path() const { return m_path; }

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

#endif // ITEMS_H
