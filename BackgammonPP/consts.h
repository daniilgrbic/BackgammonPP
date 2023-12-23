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

#endif // CONSTS_H
