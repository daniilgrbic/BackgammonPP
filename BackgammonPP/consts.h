#pragma once

#include <QString>

const qint16 MIN_USERNAME_SIZE = 4;
const qint16 MAX_USERNAME_SIZE = 20;
const qint16 MIN_NUM_GAMES = 1;
const qint16 MAX_NUM_GAMES = 10;

const qint16 BASE_THEME_VOLUME = 99;
const qint16 MAX_VOLUME = 100;

const QString THEME_SOUND_PATH = "qrc:/audio/uSnovimaProdBane.mp3";
const QString ICON_PATH = ":/resources/img/logo.png";

enum class GameType {
    ClassicGameType,
    LongNardyGameType
};

enum class PlayerType {
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
