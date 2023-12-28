#pragma once

#include "mainwindow.h"
#include "boardwindow.h"
#include "match.h"
#include "network/server.h"
#include "network/client.h"
#include <QApplication>
#include <QThread>

#include <QtMultimedia>
#include <QAudioOutput>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();

signals:
    void sendPreferences(Preferences *preferences);

public slots:
    void createGameFromMenu();
    void createGameFromMenu(QString opponentName, qint8 numGames);
    void joinRemoteGame(QString ip);
    void closeGameAndOpenMenu();
    void getPreferences(qint16 newVolume);

private:
    MainWindow *mainWindow;
    BoardWindow *boardWindow;
    Preferences *preferences;
    QMediaPlayer *themeSong;
    QAudioOutput *themeAudioOutput;
    QString playerName = Preferences().playerName;
    QString themeSongPath = THEME_SOUND_PATH;
    Server *local_server;
    Client *local_client;
    QThread *server_thread;

    void playThemeSong();
};
