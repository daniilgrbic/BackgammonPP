#include "network/server.h"
#include "network/chat_message.h"
#include "utility/jsonserializer.h"
#include <system_error>
#include <string>


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
        m_clientSockets[name] = src;

        if (!m_gameStarted && src != m_player1) {
            m_player1->write(serverCmdPotOpp+name); // send the host potential opponent
        }
    } else {
        throw std::system_error; // socket already has a name
    }
}

void Server::processOpponentCommand(QTcpSocket* src, QString oppName) {
    if (src != m_player1) {
        throw std::system_error; // only the host can choose opponent
    }
    if (m_clientNames.find(src) == m_clientNames.end()) {
        throw std::system_error; // host must submit name first
    }
    if (m_gameStarted) {
        throw std::system_error; // can't choose opponent after game has started
    }

    QMap<QTcpSocket*, QString>::iterator i;
    for (i = m_clientNames.begin(); i != m_clientNames.end(); i++) {
            QTcpSocket* spectator = *i;
            if (spectator->second == oppName) {
                m_player2 = spectator->first;
            }
    }
    
    if (m_player2 == nullptr) {
        throw std::system_error; // couldn't find player
    }
    m_spectators.removeFirst(m_player2);
    m_player1->write(serverCmdConnectedPlayer);
    m_player2->write(serverCmdConnectedPlayer);
    broadcast(serverCmdConnectedSpectator);
    m_gameStarted = true;
}

void Server::processStateCommand(QTcpSocket* src, QString state) {
    if (src != m_player1 && src != m_player2) {
        throw std::system_error; // only the host can choose opponent
    }
    if (!m_gameStarted) {
        throw std::system_error; // can't send state before the game has started
    }

    if (src == m_player1) {
        if (m_player2 == nullptr) {
            throw std::system_error; // m_player2 must exist
        }
        m_player2->write(serverCmdState + state);
    } else if (src == m_player2) {
        if (m_player1 == nullptr) {
            throw std::system_error; // m_player1 must exist
        }
        m_player1->write(serverCmdState + state);
    }
    broadcast(serverCmdState + state);
}

void Server::processChatCommand(QTcpSocket* src, QString json) {
    std::string temp = json.toStdString();
    ChatMessage chatMessage = JSONSerializer<ChatMessage>::fromJson(temp);
    chatMessage.setSender(m_clientNames[src]);
    temp = JSONSerializer<ChatMessage>::toJson(chatMessage);
    json = QString::fromUtf8(temp.c_str());

    if (src == m_player1) {
        if (m_player1 == nullptr)
            throw std::system_error;
        m_player2->write(serverCmdChat + json);
    } else if (src == m_player2) {
        if (m_player2 == nullptr)
            throw std::system_error;
        m_player1->write(serverCmdChat + json);
    } else {
        if (chatMessage.getReceiver().size() == 0) {
            broadcast(serverCmdChat + json);
        } else {
            m_clientSockets[chatMessage.getReceiver()]->write(serverCmdChat + json);
        }
    }
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
    } else if (message.startsWith(serverCmdChat)) {
        processChatCommand(sourceSocket, message.sliced(serverCmdChat.length()));
    } else { // unknown command
        throw std::system_error;
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());

    if (disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
        m_player1 = nullptr;
        m_player2 = nullptr;
        m_spectators.clear();  
        m_clientNames.clear();  
        throw std::system_error; // handle differently
    } 

    if (!m_spectators.isEmpty()) {
        if (m_clientNames.find(disconnectedSocket) != m_clientNames.end()) {
            m_clientNames.erase(m_clientNames.find(disconnectedSocket));
        }
        m_spectators.removeFirst(disconnectedSocket);
    }
}
