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

private slots:
    void open();
};
#endif // STREETMAP_H
