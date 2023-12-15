#include <cmath>
#include <string>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "streetmap.h"
#include "viewer.h"
#include "path.h"

extern void loadScene(QGraphicsScene *, const std::string &);

StreetMap::StreetMap(QWidget *parent) : QMainWindow(parent)
{
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    scene->setSceneRect(0, 0, 1280, 720);
    loadScene(scene, "map.osm");

    auto viewer = new Viewer(scene);
    setCentralWidget(viewer);

    setWindowTitle(tr("StreetMap"));

    auto openAction = new QAction(tr("&Open"), this);
    connect(openAction, &QAction::triggered, this, &StreetMap::open);

//    auto srcAction = new QAction(tr("&Select Starting Position"), this);
//    connect(srcAction, &QAction::triggered, &receiver, &Receiver::selectSource);
//    auto dstAction = new QAction(tr("Select Ending Position"), this);
//    connect(dstAction, &QAction::triggered, &receiver, &Receiver::selectDestination);
//    auto pathAction = new QAction(tr("Find Shortest Path"), this);
//    connect(pathAction, &QAction::triggered, &receiver, &Receiver::findAndShow);
    auto unshowPathAction = new QAction(tr("Unshow Path"), this);
    connect(unshowPathAction, &QAction::triggered, &receiver, &Receiver::clearPath);

    auto clrAction = new QAction(tr("Clear Selected Points"), this);
    connect(clrAction, &QAction::triggered, viewer, &Viewer::clearPoints);
    auto rmvAction = new QAction(tr("Remove Last Selected Point"), this);
    connect(rmvAction, &QAction::triggered, viewer, &Viewer::removeLastPoint);

    auto dijkstraAction = new QAction(tr("Dijkstra"), this);
    connect(dijkstraAction, &QAction::triggered, &receiver, &Receiver::dijkstra);
    auto astarAction = new QAction(tr("A*"), this);
    connect(astarAction, &QAction::triggered, &receiver, &Receiver::astar);
    auto bidAstarAction = new QAction(tr("Bidirectional A*"), this);
    connect(bidAstarAction, &QAction::triggered, &receiver, &Receiver::bidAstar);

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    auto pathMenu = menuBar()->addMenu(tr("&Path"));
//    pathMenu->addAction(srcAction);
//    pathMenu->addAction(dstAction);
//    pathMenu->addAction(pathAction);
    pathMenu->addAction(unshowPathAction);

    auto selectMenu = menuBar()->addMenu(tr("&Select"));
    selectMenu->addAction(clrAction);
    selectMenu->addAction(rmvAction);

    auto algoMenu = menuBar()->addMenu(tr("&Algorithm"));
    algoMenu->addAction(dijkstraAction);
    algoMenu->addAction(astarAction);
    algoMenu->addAction(bidAstarAction);
}

void StreetMap::open()
{
    std::string filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
    if (!filename.empty())
        loadScene(scene, filename);
}
