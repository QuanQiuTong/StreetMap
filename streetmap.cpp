#include <cmath>

#include <QWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "streetmap.h"
#include "load.h"

StreetMap::StreetMap(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    loadScene(scene);

    view = new Viewer;
    view->setScene(scene);
    setCentralWidget(view);

    setWindowTitle(tr("StreetMap"));
}

StreetMap::~StreetMap()
{
    delete scene;
    delete view;
}

