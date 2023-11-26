#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <network/server_commands.h>


class Server : public QObject {
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);
    ~Server();

public slots:
    void connected();
    void readMessage();
    void disconnected();

private:
    void broadcast(QString message);
    void processNameCommand(QTcpSocket* src, QString name);
    void processOpponentCommand(QTcpSocket* src, QString oppName);
    void processStateCommand(QTcpSocket* src, QString state);
    void processChatCommand(QTcpSocket* src, QString json);
    bool m_gameStarted;
    QTcpServer* m_server;
    QTcpSocket* m_player1;
    QTcpSocket* m_player2;
    QSet<QTcpSocket*> m_spectators;
    QMap<QTcpSocket*, QString> m_clientNames;
    QMap<QString, QTcpSocket*> m_clientSockets;
};

#endif // SERVER_H
