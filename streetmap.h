#ifndef STREETMAP_H
#define STREETMAP_H

#include <QMainWindow>
#include "viewer.h"

class QGraphicsScene;

class StreetMap : public QMainWindow
{
    Q_OBJECT

public:
    StreetMap(QWidget *parent = nullptr);
    ~StreetMap();

private:
    QGraphicsScene *scene;
    Viewer *view;
};
#endif // STREETMAP_H
