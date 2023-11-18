#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);
    ~Server();

public slots:
    void gotConnected();
    void gotMessage();
    void gotDisconnected();

private:
    QBool m_gameStarted;
    QTcpServer* m_server;
    QTcpSocket* m_player1;
    QTcpSocket* m_player2;
    QList<QTcpSocket*> m_spectators;
    QMap<QTcpSocket*, QString> m_clientNames;
};

#endif // SERVER_H
