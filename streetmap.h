#ifndef STREETMAP_H
#define STREETMAP_H

#include <QMainWindow>
#include "viewer.h"

class QGraphicsScene;
class QAction;

class StreetMap : public QMainWindow
{
    Q_OBJECT

public:
    StreetMap(QWidget *parent = nullptr);
    ~StreetMap();

private slots:
    void open();

private:
    QMenu *fileMenu, *pathMenu, *selectMenu;
    QGraphicsScene *scene;
    Viewer *viewer;
    QAction *openAction, *srcAction, *dstAction, *pathAction, *unshowPathAction, *clrAction, *rmvAction;

    void createActions();
    void createMenus();
};
#endif // STREETMAP_H
