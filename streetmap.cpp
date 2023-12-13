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

extern void loadScene(QGraphicsScene *, const std::string&);

StreetMap::StreetMap(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    loadScene(scene, "E:\\DSH-PJ\\South.osm");

    setCentralWidget(viewer = new Viewer(scene));

    setWindowTitle(tr("StreetMap"));
    createActions();
    createMenus();
}

StreetMap::~StreetMap(){
    delete scene;
    delete openAction;
    delete srcAction;
    delete dstAction;
    delete pathAction;
    delete clrAction;
    delete rmvAction;
}

void StreetMap::createActions(){
    openAction = new QAction(tr("&Open"), this);
    connect(openAction, &QAction::triggered, this, &StreetMap::open);

    srcAction = new QAction(tr("&Select Starting Position"), this);
    connect(srcAction, &QAction::triggered, &receiver, &Receiver::selectSource);
    dstAction = new QAction(tr("Select Ending Position"), this);
    connect(dstAction, &QAction::triggered, &receiver, &Receiver::selectDestination);
    pathAction = new QAction(tr("Find Shortest Path"), this);
    connect(pathAction,  &QAction::triggered, &receiver, &Receiver::findAndShow);
    unshowPathAction = new QAction(tr("Unshow Path"), this);
    connect(unshowPathAction, &QAction::triggered, &receiver, &Receiver::clearPath);

    clrAction = new QAction(tr("Clear Selected Points"), this);
    connect(clrAction, &QAction::triggered, viewer, &Viewer::clearPoints);
    rmvAction = new QAction(tr("Remove Last Selected Point"), this);
    connect(rmvAction, &QAction::triggered, viewer, &Viewer::removeLastPoint);
}

void StreetMap::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    pathMenu = menuBar()->addMenu(tr("&Path"));
    pathMenu->addAction(srcAction);
    pathMenu->addAction(dstAction);
    pathMenu->addAction(pathAction);

    selectMenu = menuBar()->addMenu(tr("&Select"));
    selectMenu->addAction(clrAction);
    selectMenu->addAction(rmvAction);
}

void StreetMap::open(){
    std::string filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
    if(!filename.empty()) loadScene(scene, filename);
}
