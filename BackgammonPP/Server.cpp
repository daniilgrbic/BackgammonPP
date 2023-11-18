#include "server.h"
#include <system_error>

/*
Player1 interaction: 

Player2 interaction:

Spectator interaction:

*/

const quint16 PORT = 1234;
const QString serverCmdOpp = "opp-";
const QString serverCmdPotOpp = "potopp-";
const QString serverCmdName = "name-";
const QString serverCmdConnectedHost = "connected-host";
const QString serverCmdConnectedSpectator = "connected-spectator";
const QString serverCmdConnectedWaiting = "connected-waiting";

Server::Server(QObject *parent)
    : QObject(parent)
{
    m_gameStarted = false;
    m_server = new QTcpServer(this);
    m_player1 = nullptr;
    m_player2 = nullptr;

    connect(m_server, &QTcpServer::newConnection, this, &Server::gotConnected);

    m_server->listen(QHostAddress::AnyIPv4, PORT);
}

Server::~Server() {
    delete m_server;
}

void Server::gotConnected() {
    QTcpSocket* connectedSocket = m_server->nextPendingConnection();
    //bool klijent1_ind = false;

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

        connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::gotMessage);
        connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::gotDisconnected);
    }

    connectedSocket->flush();
    connectedSocket->waitForBytesWritten();
}

void Server::gotMessage() {
    QTcpSocket* sourceSocket = dynamic_cast<QTcpSocket*>(sender());

    // Name resolution
    if (m_clientNames.find(sourceSocket) == m_clientNames.end()) { // socket doesn't have a name registered
        QString message = sourceSocket->readAll();

        if (message.length() < serverCmdName.length() || message.sliced(0, serverCmdName.length()) != serverCmdName) {
            throw system_error; // first message sent must be name
        }

        m_clientNames[sourceSocket] = message.sliced(5);

        if (!m_gameStarted && sourceSocket != m_player1) {
            m_player1.write(serverCmdPotOpp+m_clientNames[sourceSocket]); // send the host potential opponent
        }
        return; 
    }

    // Choosing opponent
    if (!m_gameStarted && sourceSocket == m_player1) {
        QString message = sourceSocket->readAll();

        if (message.length() < serverCmdOpp.length() || message.sliced(0, serverCmdOpp.length()) != serverCmdOpp) {
            throw system_error; // first message sent must be name
        }

        QString name = message.sliced(serverCmdOpp.length());

        for (QMap<QTcpSocket*, QString>::iterator i = m_clientNames.begin(); i != m_clientNames.end(); i++) {
            QTcpSocket* spectator = *i;
            if (spectator->second == name) {
                m_player2 == spectator->first;
                m_player1->write(serverCmdOpp+name);
            }
        }

        return;
    }

    // Sending game states
    if (sourceSocket == m_player1) {
        if (m_player2 == nullptr) {
            throw system_error; // maybe different exception
        }
        m_player2->write(sourceSocket->readAll());
    } else if (sourceSocket == m_player2) {
        if (m_player1 == nullptr) {
            throw system_error; // maybe different exception
        }
        m_player1->write(sourceSocket->readAll());
    }

    for (QList<QTcpSocket*>::iterator i = m_spectators.begin(); i != m_spectators.end(); i++) {
        QTcpSocket* spectator = *i;
        if (spectator != nullptr) {
            spectator->write(sourceSocket->readAll());
        }
    }
}

void Server::gotDisconnected() {
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
