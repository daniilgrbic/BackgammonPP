#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <system_error>
#include <network/server_commands.h>


class Client : public QObject {
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);
    ~Client();
    bool connectClient(QString ipAddress);

signals:
    void connectedAsHost();
    void connectedAsPlayer();
    void connectedAsSpectator();
    void connectedAsWaiting();
    void potentialOpponent(QString oppName);
    void newState(QString state);
    void newChatMessage(QString chatMessage);

public slots:
    void readMessage();
    void sendStateToServer(QString state);
    void sendOpponentToServer(QString state);
    void sendNameToServer(QString name);
    void sendChatMessageToServer(QString message);
    void disconnected();

private:
    QTcpSocket* m_socket;
};

#endif // CLIENT_H
