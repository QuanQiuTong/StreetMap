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

QGraphicsScene* scene;
QStatusBar* statusBar;
extern void loadScene(const std::string &);

StreetMap::StreetMap(QWidget *parent) : QMainWindow(parent)
{
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    scene->setSceneRect(0, 0, 1280, 720);
    loadScene("map.osm");

    auto viewer = new Viewer(scene);
    setCentralWidget(viewer);

    setWindowTitle(tr("StreetMap"));

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    auto openAction = new QAction(tr("&Open"));
    connect(openAction, &QAction::triggered, this, &StreetMap::open);
    fileMenu->addAction(openAction);

    auto pathMenu = menuBar()->addMenu(tr("&Path"));
    auto unshowPathAction = new QAction(tr("Unshow Path"));
    connect(unshowPathAction, &QAction::triggered, viewer, &Viewer::clearPath);
    pathMenu->addAction(unshowPathAction);

    auto selectMenu = menuBar()->addMenu(tr("&Select"));
    auto clrAction = new QAction(tr("Clear Selected Points"));
    connect(clrAction, &QAction::triggered, viewer, &Viewer::clearPoints);
    auto rmvAction = new QAction(tr("Remove Last Selected Point"));
    connect(rmvAction, &QAction::triggered, viewer, &Viewer::removeLastPoint);
    selectMenu->addAction(clrAction);
    selectMenu->addAction(rmvAction);

    auto algoMenu = menuBar()->addMenu(tr("&Algorithm"));
    auto dijkstraAction = new QAction(tr("Dijkstra"));
    connect(dijkstraAction, &QAction::triggered, viewer, &Viewer::dijk);
    auto astarAction = new QAction(tr("A*"));
    connect(astarAction, &QAction::triggered, viewer, &Viewer::astar);
    auto bidAstarAction = new QAction(tr("Bidirectional A*"));
    connect(bidAstarAction, &QAction::triggered, viewer, &Viewer::bidAstar);
    algoMenu->addAction(dijkstraAction);
    algoMenu->addAction(astarAction);
    algoMenu->addAction(bidAstarAction);

    ::statusBar = this->statusBar();
}

void StreetMap::open()
{
    std::string filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
    if (!filename.empty())
        loadScene(filename);
}
