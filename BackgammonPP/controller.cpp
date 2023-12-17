#include "controller.h"

Controller::Controller()
{
    mainWindow.show();

    this->playThemeSong();

    connect(&mainWindow, &MainWindow::requestCreateGame, this, &Controller::createGameFromMenu);
    connect(&boardWindow, &BoardWindow::requestCloseGame, this, &Controller::closeGameAndOpenMenu);
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
