#include "streetmap.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    // for(int i=0;i!=argc;++i)puts(argv[i]);
    QApplication a(argc, argv);
    StreetMap w;
    w.show();
    return a.exec();
}
