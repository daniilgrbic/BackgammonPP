#include "network/server.h"
#include "utility/jsonserializer.h"
#include "network/server_commands.h"
#include <string>


Server::Server(QString name, int numGames, GameType gameType, QObject *parent)
    : QObject(parent)
{
    m_oppName = name;
    m_lastTurn = "";
    m_gameType = gameType;
    m_numGames = numGames;
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
    for (auto spec : std::as_const(m_spectators)) {
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

    processDisconnectCommand(disconnectedSocket);
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
    else if (message.startsWith(srvconst::serverCmdDisconnect)) {
        processDisconnectCommand(sourceSocket);
    }
    else if (message == srvconst::serverCmdRequestState) {
        if (m_lastTurn != "") {
            sourceSocket->write(m_lastTurn.toStdString().c_str());
            sourceSocket->flush();
        }
    }
    else {
        throw std::runtime_error("Unknown server command");
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
    for (auto spectator : std::as_const(m_spectators)) {
        spectator->write(message.toStdString().c_str());
        spectator->flush();
    }
}

void Server::processAddNameCommand(QTcpSocket* src, QString name) {
    if (src == m_player1) {
        return;
    } else if (name == m_oppName && m_player2 == nullptr) {
        m_player2 = src;

        // let Player 2 know, type of game and number of games
        std::string message;
        switch (m_gameType) {
        case GameType::ClassicGameType:
            message = srvconst::serverCmdConnectedAsPlayerBG.toStdString() + std::to_string(m_numGames);
            break;
        case GameType::LongNardyGameType:
            message = srvconst::serverCmdConnectedAsPlayerLN.toStdString() + std::to_string(m_numGames);
            break;
        default:
            message = srvconst::serverCmdConnectedAsPlayerBG.toStdString() + std::to_string(m_numGames);
            break;
        }
        m_player2->write(message.c_str());
        m_player2->flush();

        // connected as Player 2, let Player 1 know
        m_player1->write(srvconst::serverCmdGameStart.toStdString().c_str());
        m_player1->flush();

        qDebug() << "Connected 2nd player\n";
    } else {
        // connected as Spectator
        std::string message;
        switch (m_gameType) {
        case GameType::ClassicGameType:
            message = srvconst::serverCmdConnectedAsSpectatorBG.toStdString() + std::to_string(m_numGames);
            break;
        case GameType::LongNardyGameType:
            message = srvconst::serverCmdConnectedAsSpectatorLN.toStdString() + std::to_string(m_numGames);
            break;
        default:
            message = srvconst::serverCmdConnectedAsSpectatorBG.toStdString() + std::to_string(m_numGames);
            break;
        }

        src->write(message.c_str());
        src->flush();

        m_spectators.insert(src);
        qDebug() << "Connected spectator\n";
    }
}

void Server::processRollCommand(QTcpSocket* src, QString roll) {
    broadcast(src, roll.toStdString().c_str());
}

void Server::processTurnCommand(QTcpSocket* src, QString turn) {
    m_lastTurn = turn;
    broadcast(src, turn.toStdString().c_str());
}

void Server::processDisconnectCommand(QTcpSocket *src) {
    if (src == m_player1 || src == m_player2) {
        nukeGame(src);
    } else {
        src->write(srvconst::serverCmdDisconnect.toStdString().c_str());
        src->flush();
        m_spectators.remove(src);
    }
}

void Server::nukeGame(QTcpSocket *src) {
    qDebug() << "Nuke game\n";

    if (m_player1 != src) {
        m_player1->write(srvconst::serverCmdDisconnect.toStdString().c_str());
        m_player1->flush();
        // m_player1->disconnect();
    }

    if (m_player2 != src) {
        m_player2->write(srvconst::serverCmdDisconnect.toStdString().c_str());
        m_player2->flush();
        m_player2->disconnect();
    }
    for (auto spec : std::as_const(m_spectators)) {
        spec->write(srvconst::serverCmdDisconnect.toStdString().c_str());
        spec->flush();
        spec->disconnect();
    }
    m_spectators.clear();

    // delete m_server;
}
