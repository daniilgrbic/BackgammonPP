#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <system_error>

class Client : public QObject {
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);
    ~Client();
    bool connect(QString ipAddress);

signals:
    void connectedAsHost();
    void connectedAsPlayer();
    void connectedAsSpectator();
    void connectedAsWaiting();
    void potentialOpponent(QString oppName);
    void newState(QString state);

public slots:
    void readMessage();
    void sendStateToServer(QString state);
    void sendOpponentToServer(QString state);
    void sendNameToServer(QString state);
    void disconnected();

private:
    QTcpSocket* m_socket;
};

#endif // CLIENT_H
