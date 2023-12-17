#ifndef CONSTS_H
#define CONSTS_H

#include <QString>


namespace srvconst {
    const quint16 PORT = 12345;
    inline const QString serverCmdConnected = "connected";
    inline const QString serverCmdName = "name-";
    inline const QString serverCmdSelectPlayer = "player-select-";
    inline const QString serverCmdRemovePlayer = "player-remove-";
    inline const QString serverCmdChat = "chat-";
    inline const QString serverCmdState = "state-";
    inline const QString serverCmdNames = "names-";
}


#endif // CONSTS_H
