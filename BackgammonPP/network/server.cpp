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
            m_player1->write(serverCmdConnectedHost.toStdString().c_str());
        } else {
            m_spectators.insert(connectedSocket);

            if (m_gameStarted == true) {
                connectedSocket->write(serverCmdConnectedSpectator.toStdString().c_str());
            } else {
                connectedSocket->write(serverCmdConnectedWaiting.toStdString().c_str());
            }
        }

        connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
        connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);
    }

    connectedSocket->flush();
    connectedSocket->waitForBytesWritten();
}

void Server::broadcast(QString message) {
    QSet<QTcpSocket*>::iterator i;
    for (i = m_spectators.begin(); i != m_spectators.end(); i++) {
        QTcpSocket* spectator = *i;
        if (spectator != nullptr) {
            spectator->write(message.toStdString().c_str());
        }
    }
}

void Server::processNameCommand(QTcpSocket* src, QString name) {
    if (m_clientNames.find(src) == m_clientNames.end()) { // socket doesn't have a name registered
        m_clientNames[src] = name;
        m_clientSockets[name] = src;

        if (!m_gameStarted && src != m_player1) {
            m_player1->write((serverCmdPotOpp+name).toStdString().c_str()); // send the host potential opponent
        }
    } else {
        throw std::system_error(EDOM, std::generic_category()); // socket already has a name
    }
}

void Server::processOpponentCommand(QTcpSocket* src, QString oppName) {
    if (src != m_player1) {
        throw std::system_error(EDOM, std::generic_category()); // only the host can choose opponent
    }
    if (m_clientNames.find(src) == m_clientNames.end()) {
        throw std::system_error(EDOM, std::generic_category()); // host must submit name first
    }
    if (m_gameStarted) {
        throw std::system_error(EDOM, std::generic_category()); // can't choose opponent after game has started
    }

    for (auto e : m_clientNames.keys()) {
            QTcpSocket* spectator = e;
            if (m_spectators.contains(spectator)) {

            }
    }
    if (m_clientSockets.find(oppName) != m_clientSockets.end()) {
            if (m_player2 != nullptr)
                m_spectators.insert(m_player2);

            m_player2 = m_clientSockets[oppName];
    }
    
    if (m_player2 == nullptr) {
        throw std::system_error(EDOM, std::generic_category()); // couldn't find player
    }

    m_player1->write(serverCmdConnectedPlayer.toStdString().c_str());
    m_player2->write(serverCmdConnectedPlayer.toStdString().c_str());
    broadcast(serverCmdConnectedSpectator);
    m_gameStarted = true;
}

void Server::processStateCommand(QTcpSocket* src, QString state) {
    if (src != m_player1 && src != m_player2) {
        throw std::system_error(EDOM, std::generic_category()); // only the host can choose opponent
    }
    if (!m_gameStarted) {
        throw std::system_error(EDOM, std::generic_category()); // can't send state before the game has started
    }

    if (src == m_player1) {
        if (m_player2 == nullptr) {
            throw std::system_error(EDOM, std::generic_category()); // m_player2 must exist
        }
        m_player2->write((serverCmdState + state).toStdString().c_str());
    } else if (src == m_player2) {
        if (m_player1 == nullptr) {
            throw std::system_error(EDOM, std::generic_category()); // m_player1 must exist
        }
        m_player1->write((serverCmdState + state).toStdString().c_str());
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
            throw std::system_error(EDOM, std::generic_category());
        m_player2->write((serverCmdChat + json).toStdString().c_str());
    } else if (src == m_player2) {
        if (m_player2 == nullptr)
            throw std::system_error(EDOM, std::generic_category());
        m_player1->write((serverCmdChat + json).toStdString().c_str());
    } else {
        if (chatMessage.getReceiver().size() == 0) {
            broadcast((serverCmdChat + json).toStdString().c_str());
        } else {
            m_clientSockets[chatMessage.getReceiver()]->write((serverCmdChat + json).toStdString().c_str());
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
        throw std::system_error(EDOM, std::generic_category());
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());

    if (disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
        m_player1 = nullptr;
        m_player2 = nullptr;
        m_spectators.clear();  
        m_clientNames.clear();  
        throw std::system_error(EDOM, std::generic_category()); // handle differently
    } 

    if (!m_spectators.isEmpty()) {
        if (m_clientNames.find(disconnectedSocket) != m_clientNames.end()) {
            m_clientNames.erase(m_clientNames.find(disconnectedSocket));
        }
        m_spectators.remove(disconnectedSocket);
    }
}
