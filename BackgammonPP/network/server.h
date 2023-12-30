#pragma once

#include "consts.h"

#include <QObject>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <network/server_commands.h>


class Server : public QObject {
    Q_OBJECT
public:
    Server(QString name, int numGames, GameType gameType, QObject* parent = nullptr);
    ~Server();

    void nukeGame(QTcpSocket *);

public slots:
    void connected();
    void disconnected();
    void readMessage();

private:
    void broadcast(QTcpSocket* src, QString message);
    void processAddNameCommand(QTcpSocket* src, QString name);

    void processGameStartCommand(QTcpSocket* src);

    void processRollCommand(QTcpSocket* src, QString roll);
    void processTurnCommand(QTcpSocket* src, QString turn);

    void processDisconnectCommand(QTcpSocket *);

    QString m_oppName;
    QString m_lastTurn;
    int m_numGames;
    GameType m_gameType;
    QTcpServer* m_server;
    QTcpSocket* m_player1;
    QTcpSocket* m_player2;
    QSet<QTcpSocket*> m_spectators;
};

