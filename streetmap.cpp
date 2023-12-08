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
    createActions();
    createMenus();

    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    loadScene(scene, "E:\\DSH-PJ\\South.osm");

    setCentralWidget(new Viewer(scene));

    setWindowTitle(tr("StreetMap"));
}

StreetMap::~StreetMap(){
    delete scene;
    delete openAction;
    delete srcAction;
    delete dstAction;
    delete pathAction;
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
}

void StreetMap::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    selectMenu = menuBar()->addMenu(tr("&Path"));
    selectMenu->addAction(srcAction);
    selectMenu->addAction(dstAction);
    selectMenu->addAction(pathAction);
}

void StreetMap::open(){
    std::string filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
    if(!filename.empty()) loadScene(scene, filename);
}
