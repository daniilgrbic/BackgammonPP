#ifndef CONSTS_H
#define CONSTS_H

#include <QString>

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
        soundVolume = 99;
    }
};

const qint8 MIN_USERNAME_SIZE = 8;
const qint8 MAX_USERNAME_SIZE = 20;

#endif // CONSTS_H
