#include "network/server.h"
#include "network/chat_message.h"
#include "utility/jsonserializer.h"
#include <string>


Server::Server(QObject *parent)
    : QObject(parent)
{
    m_gameStarted = false;
    m_server = new QTcpServer(this);
    m_player1 = nullptr;
    m_player2 = nullptr;

    QObject::connect(m_server, &QTcpServer::newConnection, this, &Server::connected);

    m_server->listen(QHostAddress::AnyIPv4, srvconst::PORT);
}

Server::~Server() {
    m_player1->disconnect();

    delete m_server;
}

void Server::connected() {
    QTcpSocket* connectedSocket = m_server->nextPendingConnection();


    if (connectedSocket != nullptr) {
        if (m_host == nullptr) {
            m_host = connectedSocket;
        }

        m_spectators.insert(connectedSocket);

        QObject::connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
        QObject::connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);

        connectedSocket->flush();
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket *>(QObject::sender());

    if (m_gameStarted) {
        if (disconnectedSocket == m_host || disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
            processRemovePlayerCommand(m_host, m_clientNames[m_player1]);
            processRemovePlayerCommand(m_host, m_clientNames[m_player2]);
            for (auto spectator : m_spectators)
                processRemovePlayerCommand(m_host, m_clientNames[spectator]);
            m_player1 = nullptr;
            m_player2 = nullptr;
            m_spectators.clear();
            m_clientNames.clear();
            m_clientSockets.clear();
            throw std::runtime_error(""); // handle differently
        }
        else {
            m_spectators.remove(disconnectedSocket);
            m_clientSockets.erase(m_clientSockets.find(m_clientNames[disconnectedSocket]));
            m_clientNames.erase(m_clientNames.find(disconnectedSocket));
        }
    }


}

void Server::readMessage() {
    QTcpSocket* sourceSocket = dynamic_cast<QTcpSocket*>(sender());
    QString message = sourceSocket->readAll();

    if (message.startsWith(srvconst::serverCmdName)) { // setting player name
        processNameCommand(sourceSocket, message.sliced(srvconst::serverCmdName.length()));
    } else if (message.startsWith(srvconst::serverCmdSelectPlayer)) { // choosing opponent
        processSelectPlayerCommand(sourceSocket, message.sliced(srvconst::serverCmdSelectPlayer.length()));
    } else if (message.startsWith(srvconst::serverCmdRemovePlayer)) { // choosing opponent
            processRemovePlayerCommand(sourceSocket, message.sliced(srvconst::serverCmdSelectPlayer.length()));
    } else if (message.startsWith(srvconst::serverCmdState)) { // received a game state
        processStateCommand(sourceSocket, message.sliced(srvconst::serverCmdState.length()));
    } else if (message.startsWith(srvconst::serverCmdChat)) {
        processChatCommand(sourceSocket, message.sliced(srvconst::serverCmdChat.length()));
    } else { // unknown command
        throw std::runtime_error("");
    }
}

void Server::broadcast(QTcpSocket * src, QString message, bool all) {
    if (!all) {
        if (src == m_player1) {
            if (m_player2 != nullptr) {
                m_player2->write(message.toStdString().c_str());
                m_player2->flush();
            }
        }
        else if (src == m_player2) {
            if (m_player1 != nullptr) {
                m_player1->write(message.toStdString().c_str());
                m_player1->flush();
            }
        }
        else {
            for (auto spectator : m_spectators) {
                if (spectator == src)
                    continue;
                spectator->write(message.toStdString().c_str());
                spectator->flush();
            }
        }
    }
    else {
        m_player1->write(message.toStdString().c_str());
        m_player1->flush();
        m_player2->write(message.toStdString().c_str());
        m_player2->flush();
        for (auto spectator : m_spectators) {
            spectator->write(message.toStdString().c_str());
            spectator->flush();
        }
    }
}

void Server::processNameCommand(QTcpSocket* src, QString name) {
    if (m_clientNames.find(src) == m_clientNames.end()) { // socket doesn't have a name registered
        if (m_NamesCnt[name] == 0) {
            m_clientNames[src] = name;
            m_clientSockets[name] = src;
        }
        else {
            // TODO dodavanje karaktera na postojeca imena
        }

        if (!m_gameStarted && src != m_player1) {
            m_player1->write((srvconst::serverCmdPotOpp+name).toStdString().c_str()); // send the host potential opponent
        }
    }
}

void Server::processSelectPlayerCommand(QTcpSocket* src, QString oppName) {
    if (src != m_host) {
        throw std::runtime_error("Only the host can choose opponent");
    }
    if (m_clientNames.find(src) == m_clientNames.end()) {
        throw std::runtime_error("host must submit name first");
    }
    if (m_gameStarted) {
        throw std::runtime_error("can't choose opponent after game has started");
    }

    auto keys = m_clientNames.keys();
    for (auto e : keys) {
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
            throw std::runtime_error(""); // couldn't find player
    }

    m_player1->write(srvconst::serverCmdConnectedPlayer.toStdString().c_str());
    m_player2->write(srvconst::serverCmdConnectedPlayer.toStdString().c_str());
    broadcast(src, srvconst::serverCmdConnectedSpectator);
    m_gameStarted = true;
}

void Server::processStateCommand(QTcpSocket* src, QString state) {
    if (src != m_player1 && src != m_player2) {
        throw std::runtime_error("only players can change game state");
    }
    if (!m_gameStarted) {
        throw std::runtime_error("can't send state before the game has started");
    }
    if (m_player1 == nullptr || m_player2 == nullptr) {
        throw std::runtime_error("both players must exist");
    }

    broadcast(src, srvconst::serverCmdState + state, true);
}

void Server::processChatCommand(QTcpSocket* src, QString json) {
    std::string temp = json.toStdString();
    ChatMessage chatMessage = JSONSerializer<ChatMessage>::fromJson(temp);
    chatMessage.setSender(m_clientNames[src]);
    temp = JSONSerializer<ChatMessage>::toJson(chatMessage);
    json = QString::fromUtf8(temp.c_str());

    if (src == m_player1) {
        m_player2->write((srvconst::serverCmdChat + json).toStdString().c_str());
    } else if (src == m_player2) {
        m_player1->write((srvconst::serverCmdChat + json).toStdString().c_str());
    } else {
        if (chatMessage.getReceiver().size() == 0) {
            broadcast(src, (srvconst::serverCmdChat + json).toStdString().c_str());
        } else {
            m_clientSockets[chatMessage.getReceiver()]->write((srvconst::serverCmdChat + json).toStdString().c_str());
        }
    }
}
