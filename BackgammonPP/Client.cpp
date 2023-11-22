#include "Client.h"

Client::Client(QObject* parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &Client::readMessage);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::disconnected);
}

Client::~Client() {
    delete m_socket;
}

bool Client::connect(QString ipAddress) {
    m_socket->connectToHost(QHostAddress(ipAddress), PORT);
    return m_socket->waitForConnected();
}

void Client::disconnected() {
    delete m_socket;
}

void Client::readMessage() {
    QString message = m_socket->readAll();

    if (message == serverCmdConnectedHost) {
        emit connectedAsHost();
    } else if (message == serverCmdConnectedPlayer) {
        emit connectedAsPlayer();
    } else if (message == serverCmdConnectedSpectator) {
        emit connectedAsSpectator();
    } else if (message == serverCmdConnectedWaiting) {
        emit connectedAsWaiting();
    } else if (message.startsWith(serverCmdPotOpp)) {
        emit potentialOpponent(message.sliced(serverCmdPotOpp.length()));
    } else if (message.startsWith(serverCmdState)) {
        emit newState(message.sliced(serverCmdState.length()));
    } else {
        throw system_error;
    }
}

void Client::sendStateToServer(QString state) {
    if(m_soket->state() == QAbstractSocket::ConnectedState) {
        m_soket->write(serverCmdState + state);
        m_soket->waitForBytesWritten();
    }
    else {
        throw system_error;
    }
}

void Client::sendOpponentToServer(QString oppName) {
    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(serverCmdOpp + oppName);
        m_socket->waitForBytesWritten();
    }
    else {
        throw system_error;
    }
}

void Client::sendNameToServer(QString name) {
    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(serverCmdName + name);
        m_socket->waitForBytesWritten();
    }
    else {
        throw system_error;
    }
}