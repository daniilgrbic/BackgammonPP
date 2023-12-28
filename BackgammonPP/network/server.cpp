#include "network/server.h"
#include "utility/jsonserializer.h"
#include "network/server_commands.h"
#include <string>


Server::Server(QString name, QObject *parent)
    : QObject(parent)
{
    m_oppName = name;
    m_server = new QTcpServer(this);
    m_player1 = nullptr;
    m_player2 = nullptr;

    QObject::connect(m_server, &QTcpServer::newConnection, this, &Server::connected);

    if (m_server->listen(QHostAddress::AnyIPv4, srvconst::PORT))
        qDebug() << "listening on port:" << srvconst::PORT << "\n";
    else
        qDebug() << "this port is already in use\n";
}

Server::~Server() {
    m_player1->disconnect();
    m_player2->disconnect();
    for (auto spec : m_spectators) {
        spec->disconnect();
    }
    delete m_server;
}

void Server::connected() {
    QTcpSocket* connectedSocket = m_server->nextPendingConnection();

    if (connectedSocket != nullptr) {
        if (m_player1 == nullptr) {
            m_player1 = connectedSocket;
        }

        QObject::connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
        QObject::connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);

        connectedSocket->flush();
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket *>(QObject::sender());

    if (disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
        nukeGame();
    } else {
        m_spectators.remove(disconnectedSocket);
    }
}

void Server::readMessage() {
    QTcpSocket* sourceSocket = dynamic_cast<QTcpSocket*>(sender());
    QString message = sourceSocket->readAll();

    if (message.startsWith(srvconst::serverCmdAddName)) {
        processAddNameCommand(sourceSocket, message.sliced(srvconst::serverCmdAddName.length()));
    }
    else if (message.startsWith(srvconst::serverCmdRoll)) {
        processRollCommand(sourceSocket, message);
    }
    else if (message.startsWith(srvconst::serverCmdTurn)) {
        processTurnCommand(sourceSocket, message);
    }
    else {
        throw std::runtime_error("");
    }
}

void Server::broadcast(QTcpSocket * src, QString message) {
    if (m_player1 != src) {
        m_player1->write(message.toStdString().c_str());
        m_player1->flush();
    }
    if (m_player2 != src) {
        m_player2->write(message.toStdString().c_str());
        m_player2->flush();
    }
    for (auto spectator : m_spectators) {
        spectator->write(message.toStdString().c_str());
        spectator->flush();
    }
}

void Server::processAddNameCommand(QTcpSocket* src, QString name) {
    if (src == m_player1) {
        return;
    } else if (name == m_oppName) {
        m_player2 = src;
        // connected as Player 2, let Player 1 know
        m_player1->write(srvconst::serverCmdGameStart.toStdString().c_str());
        m_player1->flush();
    } else {
        // connected as Spectator
        src->write(srvconst::serverCmdConnectedAsSpectator.toStdString().c_str());
        src->flush();
    }
}

void Server::processRollCommand(QTcpSocket* src, QString roll) {
    broadcast(src, roll.toStdString().c_str());
}

void Server::processTurnCommand(QTcpSocket* src, QString turn) {
    broadcast(src, turn.toStdString().c_str());
}

void Server::nukeGame() {
    qDebug() << "Reached\n";

    m_player1 = nullptr;
    m_player2 = nullptr;
    delete m_server;
    m_spectators.clear();

    throw std::runtime_error("Required players left!");
}
