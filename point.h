#ifndef POINT_H
#define POINT_H

#include <cmath>
// #include <map>
#include <QPointF>
#include "osm.h"

static constexpr double R = 1 << 18;
using ll = long long;


struct Point
/**
 * @brief Represents a point in a 2D coordinate system.
 */
{
    double x, y;
    static constexpr double xScale = 0.8544 * R, yScale = -R;

    /**
     * @brief Constructs a Point object with default coordinates (0, 0).
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     */
    Point(double x = 0, double y = 0) : x(x), y(y){};

    /**
     * @brief Constructs a Point object from an osm::Node object.
     * @param node The osm::Node object containing the longitude and latitude values.
     */
    Point(osm::Node node) : x(node.lon * xScale), y(node.lat * yScale) {}

    /**
     * @brief Constructs a Point object from a QPointF object.
     * @param qPointF The QPointF object containing the x and y values.
     */
    Point(QPointF qPointF) : x(qPointF.x()), y(qPointF.y()) {}

    /**
     * @brief Constructs a Point object from a given ID.
     * @param id The ID of the point.
     * @details This constructor retrieves the longitude and latitude values from the osm::nodes map using the given ID.
     * If the ID is not found in the map, an exception will be thrown.
     */
    Point(ll id)
    {
        auto [lon, lat] = osm::nodes.at(id); //'at' is const and will throw an exception if id is not found
        x = lon * xScale, y = lat * yScale;
    }

    /**
     * @brief Converts the Point object to an osm::Node object.
     * @return The osm::Node object with the converted coordinates.
     */
    operator osm::Node() const { return {x / xScale, y / yScale}; }

    /**
     * @brief Converts the Point object to a QPointF object.
     * @return The QPointF object with the converted coordinates.
     */
    operator QPointF() const { return {x, y}; }

    /**
     * @brief Checks if the Point object is valid.
     * @return True if either the x or y coordinate is non-zero, false otherwise.
     */
    operator bool() const { return x || y; }

    /**
     * @brief Checks if two Point objects are equal.
     * @param p The first Point object.
     * @param q The second Point object.
     * @return True if the x and y coordinates of both points are equal, false otherwise.
     */
    friend bool operator==(Point p, Point q) { return p.x == q.x && p.y == q.y; }
};

long long nearestPoint(Point point);
#endif // POINT_H
