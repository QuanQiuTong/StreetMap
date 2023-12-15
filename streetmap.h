#ifndef STREETMAP_H
#define STREETMAP_H

#include <QMainWindow>

class QGraphicsScene;

class StreetMap : public QMainWindow
{
    Q_OBJECT

public:
    StreetMap(QWidget *parent = nullptr);

private slots:
    void open();

private:
    QGraphicsScene *scene;
};
#endif // STREETMAP_H
