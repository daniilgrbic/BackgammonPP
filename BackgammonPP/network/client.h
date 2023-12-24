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
    void sendStateToServer(QString state);
    void sendOpponentToServer(QString state);
    void sendNameToServer(QString name);

signals:
    // these should be handled in Game
    void connectedAsHost();
    void connectedAsPlayer();
    void connectedAsSpectator();
    void connectedAsWaiting();
    void potentialOpponent(QString oppName);
    // these should be handled in Player
    void newState(QString state);


public slots:
    void readMessage();
    void disconnected();

private:
    QTcpSocket* m_socket;
};

#endif // CLIENT_H
