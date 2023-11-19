#include "mainwindow.h"
#include "boardwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BoardWindow w;
    w.show();
    return a.exec();
}
