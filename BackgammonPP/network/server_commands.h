#pragma once

#include <QString>


namespace srvconst {
    const quint16 PORT = 12345;
    inline const QString serverCmdConnectedAsSpectatorBG = "connected-spectator-bg-";
    inline const QString serverCmdConnectedAsSpectatorLN = "connected-spectator-ln-";
    inline const QString serverCmdConnectedAsPlayerBG = "connected-player-bg-";
    inline const QString serverCmdConnectedAsPlayerLN = "connected-player-ln-";
    inline const QString serverCmdAddName = "name-add-";
    inline const QString serverCmdGameStart = "game-start";
    inline const QString serverCmdRoll = "roll-";
    inline const QString serverCmdTurn = "turn-";
    inline const QString serverCmdDisconnect = "disconnect";
    inline const QString serverCmdRequestState = "request-state";
}
