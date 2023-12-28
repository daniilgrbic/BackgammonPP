#include "controller.h"

Controller::Controller()
{
    this->preferences = new Preferences();
    this->mainWindow = new MainWindow();
    this->boardWindow = new BoardWindow();

    server_thread = new QThread();
    
    mainWindow->show();

    this->playThemeSong();

    connect(mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
    connect(mainWindow, &MainWindow::requestCreateRemoteGame, this, &Controller::createRemoteGameFromMenu);
    connect(mainWindow, &MainWindow::requestJoinRemoteGame, this, &Controller::joinRemoteGame);
    connect(boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);

    connect(mainWindow, &MainWindow::requestPreferences, this, &Controller::getPreferences);
    connect(this, &Controller::sendPreferences, mainWindow, &MainWindow::handlePreferences);
}

void Controller::playThemeSong()
{
    this->themeSong = new QMediaPlayer(this);
    this->themeAudioOutput = new QAudioOutput(this);

    this->themeAudioOutput->setVolume(BASE_THEME_VOLUME);
    this->themeSong->setAudioOutput(this->themeAudioOutput);
    this->themeSong->setSource(this->themeSongPath);
    this->themeSong->setLoops(-1);

    this->themeSong->play();
}

void Controller::getPreferences(qint16 newVolume)
{
    this->themeAudioOutput->setVolume((float)newVolume / (float)MAX_VOLUME);
    this->themeSong->play();
    emit sendPreferences(this->preferences);
}

void Controller::createGameFromMenu(QString opponentName, qint8 numGames)
{
    mainWindow->close();

    boardWindow->setOpponentName(opponentName);
    boardWindow->show();
    Player *white = new LocalPlayer(nullptr, this->boardWindow);
    Player *black= new LocalPlayer(nullptr, this->boardWindow);
    Match *m = new Match(nullptr, white, black, numGames);
    white->setParent(m);
    black->setParent(m);
    m->startGame();
}

void Controller::createRemoteGameFromMenu(QString oppName)
{
    mainWindow->close();

    // boardWindow->show();
    // LocalPlayer *white = new LocalPlayer(nullptr, this->boardWindow);
    // LocalPlayer *black= new LocalPlayer(nullptr, this->boardWindow);
    // Match *m = new Match(nullptr, white, black);
    // white->setParent(m); =fiu
    // black->setParent(m);
    // m->startGame();

    local_server = new Server(oppName);

    local_server->moveToThread(server_thread);
    server_thread->start();


    boardWindow->show();
}

void Controller::joinRemoteGame(QString ip)
{
    local_client = new Client();
    local_client->connectClient(ip);
    local_client->addName(preferences->playerName);
}

void Controller::closeGameAndOpenMenu()
{
    boardWindow->close();

    if (server_thread != nullptr) {
        server_thread->quit();
        delete local_server;
        local_server = nullptr;
        qDebug() << "123\n";
    }
    if (server_thread != nullptr) {
        qDebug() << "1234\n";
    }

    mainWindow->show();
}
