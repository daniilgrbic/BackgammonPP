#pragma once

#include "consts.h"

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <network/server_commands.h>
#include <engine/core/turn.h>
#include <engine/core/roll.h>
#include <utility/jsonserializer.h>


class Client : public QObject {
    Q_OBJECT
public:
    Client(QString ipAddress, QString username, QObject* parent = nullptr);
    ~Client();
    bool connectClient(QString ipAddress);
    void disconnectClient();
    void sendTurnToServer(Turn* turn);
    void sendRollToServer(Roll roll);
    void sendNameToServer(QString name);
    QTcpSocket* getSocket();

signals:
    void connectedAsSpectator(int length, GameType gameType);
    void connectedAsPlayer(int length, GameType gameType);
    void disconnected();
    void unknownServerCommand(QString srvCmd);
    void notConnected();
    void startGame();

    void sendMove(Turn turn);
    void diceRolled(Roll roll);

public slots:
    void readMessageFromServer();
    void disconnectedFromServer();

private:
    QTcpSocket* m_socket;
};
