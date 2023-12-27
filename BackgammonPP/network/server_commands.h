#pragma once

#include <QString>


namespace srvconst {
    const quint16 PORT = 12345;
    inline const QString serverCmdConnected = "connected";
    inline const QString serverCmdAddName = "name-add-";
    inline const QString serverCmdRemoveName = "name-remove-";
    inline const QString serverCmdSelectPlayer = "player-select-";
    inline const QString serverCmdRemovePlayer = "player-remove-";
    inline const QString serverCmdState = "state-";
    inline const QString serverCmdChat = "chat-";
    inline const QString serverCmdDoubling = "doubling";
    inline const QString serverCmdDice = "dice-";
    inline const QString serverCmdGameOn = "game-on";
    inline const QString serverCmdGameStart = "game-start";
    inline const QString serverCmdGameEnd = "game-end";
}
