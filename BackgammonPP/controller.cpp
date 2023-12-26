#include "controller.h"

Controller::Controller()
{
    this->preferences = new Preferences();
    this->mainWindow = new MainWindow();
    this->boardWindow = new BoardWindow();
    //mainWindow->show();
    boardWindow->show();
    LocalPlayer *white = new LocalPlayer(nullptr, this->boardWindow);
    LocalPlayer *black= new LocalPlayer(nullptr, this->boardWindow);
    Match *m = new Match(nullptr, white, black);
    white->setParent(m);
    black->setParent(m);
    m->startGame();


    this->playThemeSong();

    connect(mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
    connect(boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);

    connect(mainWindow, &MainWindow::requestPreferences, this, &Controller::getPreferences);
    connect(this, &Controller::sendPreferences, mainWindow, &MainWindow::handlePreferences);
}

void Controller::playThemeSong()
{
    /* TODO: CAN'T IMPORT LIBRARIES
    mediaPlayer = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(mediaPlayer);

    playlist->addMedia(QUrl(this->themeSongPath));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaPlayer->setPlaylist(playlist);
    mediaPlayer->play();

    QTimer *checkTimer = new QTimer(this);
    connect(checkTimer, &QTimer::timeout, this, &SoundWidget::checkIfAudioStillPlaying);
    checkTimer->start(1000);
    */
}

void Controller::getPreferences()
{
    emit sendPreferences(this->preferences);
}

void Controller::createGameFromMenu()
{
    mainWindow->close();
    boardWindow->show();
}

void Controller::closeGameAndOpenMenu()
{
    boardWindow->close();
    mainWindow->show();
}
