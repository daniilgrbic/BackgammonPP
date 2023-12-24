#pragma once

#include <QString>

const qint16 MIN_USERNAME_SIZE = 5;
const qint16 MAX_USERNAME_SIZE = 20;
const qint16 MIN_NUM_GAMES = 1;
const qint16 MAX_NUM_GAMES = 10;

const qint16 BASE_THEME_VOLUME = 99;
const qint16 MAX_VOLUME = 100;

const QString THEME_SOUND_PATH = "qrc:/audio/uSnovimaProdBane.mp3";

enum GameType {
    ClassicGameType,
    LongNardyGameType
};

enum PlayerType {
    BotPlayer,
    LocalPlayer,
    RemotePlayer
};

struct Preferences {
    QString playerName;
    qint32 soundVolume;
    Preferences() {
        playerName = "Player";
        soundVolume = MAX_VOLUME;
    }
};
