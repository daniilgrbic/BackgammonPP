#include "network/client.h"


Client::Client(bool host, QObject* parent)
    : QObject(parent)
    , m_host{host}
{
    m_socket = new QTcpSocket(this);

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readMessageFromServer);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &Client::disconnectedFromServer);
}

Client::~Client() {
    delete m_socket;
}

bool Client::connectClient(QString ipAddress) {
    m_socket->connectToHost(QHostAddress(ipAddress), srvconst::PORT);
    return m_socket->waitForConnected();
}

void Client::readMessageFromServer() {
    QString message = m_socket->readAll();

    if (message == srvconst::serverCmdConnected) {
        emit connected(message);
    } else if (message.startsWith(srvconst::serverCmdAddName)) {
        emit addName(message.sliced(srvconst::serverCmdAddName.length()));
    } else if (message.startsWith(srvconst::serverCmdRemoveName)) {
        emit removeName(message.sliced(srvconst::serverCmdRemoveName.length()));
    } else if (message.startsWith(srvconst::serverCmdState)) {
        emit newState(message.sliced(srvconst::serverCmdState.length()));
    } else if (message.startsWith(srvconst::serverCmdChat)) {
        emit newChatMessage(message.sliced(srvconst::serverCmdChat.length()));
    } else {
        emit unknownServerCommand(message);
    }
}

void Client::disconnectedFromServer() {
    emit disconnected();
}

void Client::sendStateToServer(QString state) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write((srvconst::serverCmdState + state).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
}

void Client::sendPlayerToServer(QString oppName) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write((srvconst::serverCmdSelectPlayer + oppName).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
}

void Client::sendNameToServer(QString name) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write((srvconst::serverCmdAddName + name).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
}
