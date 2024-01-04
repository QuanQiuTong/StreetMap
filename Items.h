#ifndef ITEMS_H
#define ITEMS_H

#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include "viewer.h"
#include "path.h"
using path::SRC, path::DST, path::srcPos, path::dstPos;

/**
 * @brief Represents a waypoint on a map.
 *
 * The WayPoint class is a QGraphicsEllipseItem that represents a waypoint on a map. It inherits from QGraphicsEllipseItem and provides additional functionality for handling mouse events and painting the waypoint on the scene.
 */
class WayPoint : public QGraphicsEllipseItem
{
    long long id;

public:
    /**
     * @brief Constructs a WayPoint object.
     *
     * @param p The position of the waypoint.
     * @param _id The ID of the waypoint.
     * @param radius The radius of the waypoint.
     * @param parent The parent item of the waypoint.
     */
    WayPoint(Point p, long long _id, double radius = 5, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(p.x - radius, p.y - radius, 2 * radius, 2 * radius, parent), id(_id) { setFlags(ItemIsSelectable); }

    /**
     * @brief Paints the waypoint on the scene.
     *
     * @param painter The QPainter object used for painting.
     * @param option The style options for the waypoint.
     * @param widget The widget being painted on.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::white);
        painter->drawEllipse(rect());
    }

protected:
    /**
     * @brief Handles the mouse press event for the waypoint.
     *
     * @param event The mouse event.
     */
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

/**
 * @brief Represents a path item in a street map.
 *
 * This class inherits from QGraphicsPathItem and provides additional functionality for painting the path.
 */
struct Path : public QGraphicsPathItem
{
    Path(QPainterPath path, QGraphicsItem *parent = nullptr) : QGraphicsPathItem(path, parent) {}

    /**
     * @brief Paints the path item.
     *
     * This function is called by the graphics framework to paint the path item on the screen.
     * It sets the pen color and width based on the current transformation matrix and then draws the path.
     *
     * @param painter The painter object used for painting.
     * @param option The style options for the item.
     * @param widget The widget on which the item is painted.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setPen({Qt::red, 5 / std::min(painter->transform().m11(), 1.0)});
        painter->drawPath(path());
    }
};
extern Path *shortPath;

/**
 * @brief Represents a selectable point in a graphics scene.
 *
 * This class inherits from QGraphicsItem and provides functionality to display and interact with a point on a graphics scene.
 * The point can be selected or deselected, and its appearance changes accordingly.
 */
class SelectPoint : public QGraphicsItem
{
    Point point;
    double radius;

public:
    /**
     * @brief Constructs a SelectPoint object with the given position and parent item.
     *
     * @param p The position of the point.
     * @param parent The parent item of the SelectPoint object.
     */
    SelectPoint(QPointF p, QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), point(p), radius(6.0) {}

    /**
     * @brief Paints the point on the graphics scene.
     *
     * @param painter The QPainter object used for painting.
     * @param option The style options for the item.
     * @param widget The widget on which the item is painted.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        radius = 6.0 / std::min(painter->transform().m11(), 1.0);
        painter->setPen(Qt::lightGray);
        painter->setBrush((option->state & QStyle::State_Selected) ? Qt::green : Qt::red);
        painter->drawEllipse(point, radius, radius);
    }

    /**
     * @brief Returns the bounding rectangle of the point.
     *
     * @return The bounding rectangle of the point.
     */
    QRectF boundingRect() const override
    {
        return {point.x - radius, point.y - radius, 2 * radius, 2 * radius};
    }

    /**
     * @brief Returns the shape of the point as a QPainterPath.
     *
     * @return The shape of the point as a QPainterPath.
     */
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }

    /**
     * @brief Handles the mouse press event on the point.
     *
     * @param event The mouse event.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *) override
    {
        using namespace path;
        if (!SRC)
            SRC = nearestPoint(point), srcPos = point;
        else if (!DST)
            DST = nearestPoint(point), dstPos = point, findAndShow();
        else
            SRC = DST, srcPos = dstPos, DST = nearestPoint(point), dstPos = point, findAndShow();
    }
};

#endif // ITEMS_H
