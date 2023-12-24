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
    void createGameFromMenu();
    void closeGameAndOpenMenu();
    void getPreferences(qint16 newVolume);

private:
    MainWindow *mainWindow;
    BoardWindow *boardWindow;
    Preferences *preferences;
    QMediaPlayer *themeSong;
    QAudioOutput *themeAudioOutput;
    QString playerName = "Player";
    QString themeSongPath = "qrc:/audio/uSnovimaProdBane.mp3";

    void playThemeSong();
};
