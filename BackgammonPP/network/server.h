#pragma once

#include <QObject>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <network/server_commands.h>


class Server : public QObject {
    Q_OBJECT
public:
    Server(QString name, QObject* parent = nullptr);
    ~Server();

public slots:
    void connected();
    void disconnected();
    void readMessage();

private:
    void broadcast(QTcpSocket* src, QString message, bool all = false);

    void processAddNameCommand(QTcpSocket* src, QString name);
    void processRemoveNameCommand(QTcpSocket* src, QString name);
    void processSelectPlayerCommand(QTcpSocket* src, QString name);
    void processRemovePlayerCommand(QTcpSocket* src, QString name);

    void processStateCommand(QTcpSocket* src, QString state);
    void processChatCommand(QTcpSocket* src, QString json);
    void processDoublingCommand(QTcpSocket* src);
    void processDiceCommand(QTcpSocket* src, QString cmd);

    void processGameStartCommand(QTcpSocket* src);
    void processGameEndCommand(QTcpSocket* src);

    void nukeGame();
    void removeName(QTcpSocket *);

    QString oppName;
    bool m_gameOn;
    QTcpServer* m_server;
    QTcpSocket* m_host;
    QTcpSocket* m_player1;
    QTcpSocket* m_player2;
    QSet<QTcpSocket*> m_spectators;
    QMap<QTcpSocket*, QString> m_clientNames;
    QMap<QString, QTcpSocket*> m_clientSockets;
    QMap<QString, int> m_namesCnt;
};

