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
    QMenu *fileMenu, *selectMenu;
    QGraphicsScene *scene;
    QAction* openAction, *srcAction, *dstAction, *pathAction;

    void createActions();
    void createMenus();
};
#endif // STREETMAP_H
