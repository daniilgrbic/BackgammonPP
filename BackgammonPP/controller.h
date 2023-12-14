#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mainwindow.h"
#include "boardwindow.h"
#include <QApplication>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();

public slots:
    void createGameFromMenu();
    void closeGameAndOpenMenu();

private:
    MainWindow mainWindow;
    BoardWindow boardWindow;
};

#endif // CONTROLLER_H
