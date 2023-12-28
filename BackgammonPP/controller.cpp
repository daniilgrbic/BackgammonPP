#include "controller.h"
#include "match.h"
#include "localplayer.h"

Controller::Controller()
{
    this->preferences = new Preferences();
    this->mainWindow = new MainWindow();
    this->boardWindow = new BoardWindow();

    mainWindow->show();

    this->playThemeSong();

    connect(mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
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

void Controller::createGameFromMenu(QString opponentName, qint8 numGames, GameType gameType)
{
    mainWindow->close();

    boardWindow->setOpponentName(opponentName);
    boardWindow->show();
    Player *white = new LocalPlayer(nullptr, this->boardWindow);
    Player *black= new LocalPlayer(nullptr, this->boardWindow);
    Match *m = new Match(nullptr, white, black, numGames, gameType);
    white->setParent(m);
    black->setParent(m);
    m->startGame();
}

void Controller::closeGameAndOpenMenu()
{
    boardWindow->close();
    mainWindow->show();
}
