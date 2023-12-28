#pragma once

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
    void sendTurnToServer(Turn* turn);
    void sendRollToServer(Roll roll);
    void sendNameToServer(QString name);

signals:
    void connectedAsSpectator(QString msg);
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

//     bool connectClient(QString ipAddress);
//     void sendStateToServer(QString state);
//     void sendOpponentToServer(QString state);
//     void sendNameToServer(QString name);

// signals:
//     // these should be handled in Game
//     void connectedAsHost();
//     void connectedAsPlayer();
//     void connectedAsSpectator();
//     void connectedAsWaiting();
//     void potentialOpponent(QString oppName);
//     // these should be handled in Player
//     void newState(QString state);


// public slots:
//     void readMessage();
//     void disconnected();
