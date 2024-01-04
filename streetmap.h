#ifndef STREETMAP_H
#define STREETMAP_H

#include <QMainWindow>

class QGraphicsScene;
extern QGraphicsScene *scene;
extern QStatusBar *statusBar;
class StreetMap : public QMainWindow
{
    Q_OBJECT

public:
    StreetMap(QWidget *parent = nullptr);
};
#endif // STREETMAP_H
