#pragma once

#include "mainwindow.h"
#include "boardwindow.h"
#include <QApplication>


// FIXME: CAN'T IMPORT LIBRARIES
// #include <QMediaPlayer>
// #include <QMediaPlaylist>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();

signals:
    void sendPreferences(Preferences *preferences);

public slots:
    void createGameFromMenu();
    void closeGameAndOpenMenu();
    void getPreferences();

private:
    MainWindow *mainWindow;
    BoardWindow *boardWindow;
    Preferences *preferences;
    QString playerName = "Player";
    QString themeSongPath = ":/music/audio/skip_james-devil_got_my_woman.mp3";

    void playThemeSong();
};
