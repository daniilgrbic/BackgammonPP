#pragma once

#include <QString>


namespace srvconst {
const quint16 PORT = 12345;
    inline const QString serverCmdConnectedAsSpectator = "connected-as-spectator";
    inline const QString serverCmdAddName = "name-add-";
    inline const QString serverCmdGameStart = "game-start";
    inline const QString serverCmdRoll = "roll-";
    inline const QString serverCmdTurn = "turn-";
    inline const QString serverCmdDisconnect = "disconnect";
}
