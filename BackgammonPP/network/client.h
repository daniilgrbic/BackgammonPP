#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <network/server_commands.h>


class Client : public QObject {
    Q_OBJECT
public:
    Client(bool host = false, QObject* parent = nullptr);
    ~Client();
    bool connectClient(QString ipAddress);

signals:
    void connected(QString msg);
    void disconnected();
    void addName(QString name);
    void removeName(QString name);
    void selectPlayer(QString name);
    void removePlayer(QString name);
    void newState(QString state);
    void newChatMessage(QString chatMessage);
    void unknownServerCommand(QString srvCmd);
    void notConnected();


public slots:
    void readMessageFromServer();
    void disconnectedFromServer();

    void sendStateToServer(QString state);
    void sendPlayerToServer(QString state);
    void sendNameToServer(QString state);


private:
    QTcpSocket* m_socket;
    bool m_host;
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
