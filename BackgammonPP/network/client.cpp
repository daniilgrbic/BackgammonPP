#include <QNetworkProxy>
#include "network/client.h"


Client::Client(QString ipAddress, QString username, bool host, QObject* parent)
    : QObject(parent)
    , m_host{host}
{
    m_socket = new QTcpSocket(this);
    m_socket->setProxy(QNetworkProxy::NoProxy);

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readMessageFromServer);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &Client::disconnectedFromServer);

    connectClient(ipAddress);
    sendNameToServer(username);
}

Client::~Client() {
    delete m_socket;
}

bool Client::connectClient(QString ipAddress) {
    m_socket->connectToHost(QHostAddress(ipAddress), srvconst::PORT);
    return m_socket->waitForConnected();
}

void Client::sendTurnToServer(Turn* turn) {
    QString toServer = srvconst::serverCmdTurn + QString::fromUtf8(JSONSerializer<Turn>::toJson(*turn));
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(toServer.toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
}

void Client::sendRollToServer(Roll roll) {
    // QString toServer = srvconst::serverCmdRoll + QString::fromUtf8(JSONSerializer<Roll>::toJson(roll));
    QString  toServer;
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(toServer.toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
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
    } else if (message.startsWith(srvconst::serverCmdRoll)) {
        message = message.sliced(srvconst::serverCmdRoll.length());
        std::string temp = message.toStdString();
        // Roll roll = JSONSerializer<Roll>::fromJson(temp);
        // emit diceRolled(roll);
    } else if (message.startsWith(srvconst::serverCmdTurn)) {
        message = message.sliced(srvconst::serverCmdTurn.length());
        std::string temp = message.toStdString();
        Turn turn = JSONSerializer<Turn>::fromJson(temp);
        emit acceptMove(turn);
    }
    else {
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

void Client::sendPlayerToServer(QString player) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write((srvconst::serverCmdSelectPlayer + player).toStdString().c_str());
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

void Client::sendRollToServer(QString roll) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write((srvconst::serverCmdRoll + roll).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else {
        emit notConnected();
    }
}
