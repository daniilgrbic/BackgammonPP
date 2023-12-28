#pragma once

#include "mainwindow.h"
#include "boardwindow.h"
#include <QApplication>

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
    void getPreferences(qint16 newVolume);

private:
    MainWindow *mainWindow;
    BoardWindow *boardWindow;
    Preferences *preferences;
    QMediaPlayer *themeSong;
    QAudioOutput *themeAudioOutput;
    QString playerName = Preferences().playerName;
    QString themeSongPath = THEME_SOUND_PATH;

    void playThemeSong();
};
