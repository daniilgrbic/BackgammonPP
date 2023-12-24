#ifndef CONSTS_H
#define CONSTS_H

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
    inline const QString serverCmdGameOn = "game-on";
    inline const QString serverCmdGameStart = "game-start";
}

#endif // CONSTS_H
