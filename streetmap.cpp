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
#include "Items.h"

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
    connect(openAction, &QAction::triggered, this, [this]
            {std::string filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
             if (!filename.empty()) loadScene(filename);});
    fileMenu->addAction(openAction);

    auto pathMenu = menuBar()->addMenu(tr("&Path"));
    auto unshowPathAction = new QAction(tr("Unshow Path"));
    connect(unshowPathAction, &QAction::triggered, viewer, &Viewer::clearPath);
    pathMenu->addAction(unshowPathAction);

    auto selectMenu = menuBar()->addMenu(tr("&Select"));
    auto clrAction = new QAction(tr("Clear Selected Points"));
    connect(clrAction, &QAction::triggered, scene,[]
            {while (!selectedPoints.empty()) scene->removeItem(selectedPoints.back()), delete selectedPoints.back(), selectedPoints.pop_back();});
    auto rmvAction = new QAction(tr("Remove Last Selected Point"));
    connect(rmvAction, &QAction::triggered, scene,[]
            {if (!selectedPoints.empty()) scene->removeItem(selectedPoints.back()), delete selectedPoints.back(), selectedPoints.pop_back();});
    selectMenu->addAction(clrAction);
    selectMenu->addAction(rmvAction);

    auto algoMenu = menuBar()->addMenu(tr("&Algorithm"));
    auto dijkstraAction = new QAction(tr("Dijkstra"));
    connect(dijkstraAction, &QAction::triggered, viewer, []{ findShortestPath = path::dijkstra, findAndShow(); });
    auto astarAction = new QAction(tr("A*"));
    connect(astarAction, &QAction::triggered, viewer, []{ findShortestPath = path::aStar, findAndShow(); });
    auto bidAstarAction = new QAction(tr("Bidirectional A*"));
    connect(bidAstarAction, &QAction::triggered, viewer, []{ findShortestPath = path::bidirectionalAStar, findAndShow(); });
    algoMenu->addAction(dijkstraAction);
    algoMenu->addAction(astarAction);
    algoMenu->addAction(bidAstarAction);

    ::statusBar = this->statusBar();
}
