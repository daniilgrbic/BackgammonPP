#include <QNetworkProxy>
#include "network/client.h"


Client::Client(QString ipAddress, QString username, QObject* parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readMessageFromServer);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &Client::disconnectedFromServer);

    connectClient(ipAddress);
    sendNameToServer(username);
}

Client::~Client() {
    delete m_socket;
}

QTcpSocket* Client::getSocket() {
    return m_socket;
}

bool Client::connectClient(QString ipAddress) {
    m_socket->connectToHost(QHostAddress(ipAddress), srvconst::PORT);
    return m_socket->waitForConnected();
}

void Client::disconnectClient() {
    if (m_socket != nullptr && m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->write(srvconst::serverCmdDisconnect.toStdString().c_str());
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
    QString toServer = srvconst::serverCmdRoll + QString::fromUtf8(JSONSerializer<Roll>::toJson(roll));
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(toServer.toStdString().c_str());
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

void Client::readMessageFromServer() {
    QString message = m_socket->readAll();

    if (message == srvconst::serverCmdGameStart) {
        emit startGame();
    } else if (message.startsWith(srvconst::serverCmdConnectedAsSpectatorBG)) {
        message = message.sliced(srvconst::serverCmdConnectedAsSpectatorBG.length());
        std::string temp = message.toStdString();
        emit connectedAsSpectator(stoi(temp), GameType::ClassicGameType);

        m_socket->write((srvconst::serverCmdRequestState).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else if (message.startsWith(srvconst::serverCmdConnectedAsSpectatorLN)) {
        message = message.sliced(srvconst::serverCmdConnectedAsSpectatorLN.length());
        std::string temp = message.toStdString();
        emit connectedAsSpectator(stoi(temp), GameType::LongNardyGameType);

        m_socket->write((srvconst::serverCmdRequestState).toStdString().c_str());
        m_socket->waitForBytesWritten();
    } else if (message.startsWith(srvconst::serverCmdConnectedAsPlayerBG)) {
        message = message.sliced(srvconst::serverCmdConnectedAsPlayerBG.length());
        std::string temp = message.toStdString();
        emit connectedAsPlayer(stoi(temp), GameType::ClassicGameType);
    } else if (message.startsWith(srvconst::serverCmdConnectedAsPlayerLN)) {
        message = message.sliced(srvconst::serverCmdConnectedAsPlayerLN.length());
        std::string temp = message.toStdString();
        emit connectedAsPlayer(stoi(temp), GameType::LongNardyGameType);
    } else if (message.startsWith(srvconst::serverCmdRoll)) {
        message = message.sliced(srvconst::serverCmdRoll.length());
        std::string temp = message.toStdString();
        Roll roll = JSONSerializer<Roll>::fromJson(temp);
        emit diceRolled(roll);
    } else if (message.startsWith(srvconst::serverCmdTurn)) {
        message = message.sliced(srvconst::serverCmdTurn.length());
        std::string temp = message.toStdString();
        Turn turn = JSONSerializer<Turn>::fromJson(temp);
        emit sendMove(turn);
    } else if (message.startsWith(srvconst::serverCmdDisconnect)) {
        m_socket->disconnect();
        m_socket = nullptr;
        disconnectedFromServer();
    } else {
        emit unknownServerCommand(message);
    }
}

void Client::disconnectedFromServer() {
    emit disconnected();
}
