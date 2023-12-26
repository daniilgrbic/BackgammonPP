#pragma once
#include <QString>

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
        soundVolume = 99;
    }
};

const qint16 MIN_USERNAME_SIZE = 8;
const qint16 MAX_USERNAME_SIZE = 20;
const qint16 MIN_NUM_MOVES = 10;
const qint16 MAX_NUM_MOVES = 200;

