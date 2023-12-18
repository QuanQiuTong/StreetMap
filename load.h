#ifndef LOAD_H
#define LOAD_H

#include "osm.h"

void loadClosedWay(const osm::Way &way);
void loadOpenWay(const osm::Way &way);

#endif