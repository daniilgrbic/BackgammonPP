#pragma once

#include "mainwindow.h"
#include "boardwindow.h"
#include "network/server.h"
#include "match.h"
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
    void createGameFromMenu(QString opponentName, qint8 numGames, GameType gameType, PlayerType playerType);
    void closeGameAndOpenMenu();
    void joinRemoteMatchFromMenu(QString ipAddress);
    void getPreferences(qint16 newVolume);

private:
    MainWindow *mainWindow;
    BoardWindow *boardWindow;
    Preferences *preferences;
    QMediaPlayer *themeSong;
    QAudioOutput *themeAudioOutput;
    QString themeSongPath = THEME_SOUND_PATH;

    Server *server_local = nullptr;
    QThread *thread_server = nullptr;
    Player *player_remote = nullptr;

    Match *match_current = nullptr;

    void playThemeSong();
};
