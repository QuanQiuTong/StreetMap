#include <cmath>

#include <QWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "streetmap.h"
#include "load.h"

StreetMap::StreetMap(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();

    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor(255, 255, 238));
    filename = "E:\\DSH-PJ\\South.osm";
    loadScene(scene, filename);

    view = new Viewer;
    view->setScene(scene);
    setCentralWidget(view);


    setWindowTitle(tr("StreetMap"));
}

StreetMap::~StreetMap()
{
    delete scene;
    delete view;
    delete openAction;
    delete srcAction;
    delete dstAction;
}

void StreetMap::createActions(){
    openAction = new QAction(tr("&Open"),this);
    connect(openAction, SIGNAL(triggered()),this, SLOT(open()));

    srcAction = new QAction(tr("&Select Starting Position"),this);
    connect(srcAction, SIGNAL(triggered()), this, SLOT(selectPoint(1)));
    dstAction = new QAction(tr("Select Ending Position"),this);
    connect(dstAction, SIGNAL(triggered()), this, SLOT(selectPoint(2)));
}

void StreetMap::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);

    selectMenu = menuBar()->addMenu(tr("&Select"));
    selectMenu->addAction(srcAction);
    selectMenu->addAction(dstAction);
}

void StreetMap::open(){
    filename = QFileDialog::getOpenFileName(this, tr("Select Map"), ".", tr("OpenStreetMap files (*.osm)")).toStdString();
    loadScene(scene, filename);
}

void StreetMap::selectPoint(int type){

}
