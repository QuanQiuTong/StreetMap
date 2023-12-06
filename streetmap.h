#ifndef STREETMAP_H
#define STREETMAP_H

#include <string>

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
    void selectPoint(int type);

private:
    QMenu *fileMenu, *selectMenu;
    QGraphicsScene *scene;
    Viewer *view;
    QAction* openAction, *srcAction, *dstAction;
    std::string filename;

    void createActions();
    void createMenus();
};
#endif // STREETMAP_H
