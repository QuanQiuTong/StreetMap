#include "osm.h"
#include <cmath>
using namespace osm;
double _dist(Node p, Node q)
{
    return hypot(p.lon - q.lon, p.lat - q.lat) * (M_PI * 6378137 / 180);
}
static double rad(double deg) { return deg * M_PI / 180; };
double dist(Node p, Node q)
{
    double dLat = rad(q.lat - p.lat);
    double dLon = rad(q.lon - p.lon);
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(rad(p.lat)) * cos(rad(q.lat)) *
                   sin(dLon / 2) * sin(dLon / 2);
    return 2 * 6378137 * asin(sqrt(a));
}
int main(){
    //121.910310, 30.858038, 122.011323, 30.663016
    Node p={121.910310, 30.858038},q={122.011323, 30.663016};
    printf("%lf %lf", dist(p,q), _dist(p,q));
}