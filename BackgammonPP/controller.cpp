#include "controller.h"

Controller::Controller()
{
    mainWindow.show();

    connect(&mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
    connect(&boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);
}

void Controller::createGameFromMenu()
{
    mainWindow.close();
    boardWindow.show();
}

void Controller::closeGameAndOpenMenu()
{
    boardWindow.close();
    mainWindow.show();
}
