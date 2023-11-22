#include "server.h"

Server::Server(QObject *parent)
    : QObject(parent)
{
    m_gameStarted = false;
    m_server = new QTcpServer(this);
    m_player1 = nullptr;
    m_player2 = nullptr;

    connect(m_server, &QTcpServer::newConnection, this, &Server::connected);

    m_server->listen(QHostAddress::AnyIPv4, PORT);
}

Server::~Server() {
    delete m_server;
}

void Server::connected() {
    QTcpSocket* connectedSocket = m_server->nextPendingConnection();

    if (connectedSocket != nullptr) {
        if (m_player1 == nullptr) {
            m_player1 = connectedSocket;
            m_player1->write(serverCmdConnectedHost);
        } else {
            m_spectators.push_back(connectedSocket);

            if (m_gameStarted == true) {
                connectedSocket->write(serverCmdConnectedSpectator);
            } else {
                connectedSocket->write(serverCmdConnectedWaiting);
            }
        }

        connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
        connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);
    }

    connectedSocket->flush();
    connectedSocket->waitForBytesWritten();
}

void Server::broadcast(QString message) {
    QList<QTcpSocket*>::iterator i;
    for (i = m_spectators.begin(); i != m_spectators.end(); i++) {
        QTcpSocket* spectator = *i;
        if (spectator != nullptr) {
            spectator->write(message);
        }
    }
}

void Server::processNameCommand(QTcpSocket* src, QString name) {
    if (m_clientNames.find(src) == m_clientNames.end()) { // socket doesn't have a name registered
        m_clientNames[src] = name;

        if (!m_gameStarted && src != m_player1) {
            m_player1->write(serverCmdPotOpp+name); // send the host potential opponent
        }
    } else {
        throw system_error; // socket already has a name
    }
}

void Server::processOpponentCommand(QTcpSocket* src, QString oppName) {
    if (src != m_player1) {
        throw system_error; // only the host can choose opponent
    }
    if (m_clientNames.find(src) == m_clientNames.end()) {
        throw system_error; // host must submit name first
    }
    if (m_gameStarted) {
        throw system_error; // can't choose opponent after game has started
    }

    QMap<QTcpSocket*, QString>::iterator i;
    for (i = m_clientNames.begin(); i != m_clientNames.end(); i++) {
            QTcpSocket* spectator = *i;
            if (spectator->second == oppName) {
                m_player2 = spectator->first;
            }
    }
    
    if (m_player2 == nullptr) {
        throw system_error; // couldn't find player
    }
    m_spectators.removeFirst(m_player2);
    m_player1->write(serverCmdConnectedPlayer);
    m_player2->write(serverCmdConnectedPlayer);
    broadcast(serverCmdConnectedSpectator);
    m_gameStarted = true;
}

void Server::processStateCommand(QTcpSocket* src, QString state) {
    if (src != m_player1 && src != m_player2) {
        throw system_error; // only the host can choose opponent
    }
    if (!m_gameStarted) {
        throw system_error; // can't send state before the game has started
    }

    if (src == m_player1) {
        if (m_player2 == nullptr) {
            throw system_error; // m_player2 must exist
        }
        m_player2->write(serverCmdState + state);
    } else if (src == m_player2) {
        if (m_player1 == nullptr) {
            throw system_error; // m_player1 must exist
        }
        m_player1->write(serverCmdState + state);
    }
    broadcast(serverCmdState + state);
}

void Server::readMessage() {
    QTcpSocket* sourceSocket = dynamic_cast<QTcpSocket*>(sender());
    QString message = sourceSocket->readAll();

    if (message.startsWith(serverCmdName)) { // setting player name
        processNameCommand(sourceSocket, message.sliced(serverCmdName.length())); 
    } else if (message.startsWith(serverCmdOpp)) { // choosing opponent
        processOpponentCommand(sourceSocket, message.sliced(serverCmdOpp.length())); 
    } else if (message.startsWith(serverCmdState)) { // received a game state
        processStateCommand(sourceSocket, message.sliced(serverCmdState.length()));
    } else { // unknown command
        throw system_error; 
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());

    if (disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
        m_player1 = nullptr;
        m_player2 = nullptr;
        m_spectators.clear();  
        m_clientNames.clear();  
        throw system_error; // handle differently
    } 

    if (!m_spectators.isEmpty()) {
        if (m_clientNames.find(disconnectedSocket) != m_clientNames.end()) {
            m_clientNames.erase(m_clientNames.find(disconnectedSocket));
        }
        m_spectators.removeFirst(disconnectedSocket);
    }
}
