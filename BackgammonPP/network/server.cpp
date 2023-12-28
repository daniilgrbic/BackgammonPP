#include "network/server.h"
#include "network/chat_message.h"
#include "utility/jsonserializer.h"
#include "network/server_commands.h"
#include <string>


Server::Server(QString name, QObject *parent)
    : QObject(parent)
{
    oppName = name;
    m_gameOn = false;
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

    delete m_server;
}

void Server::connected() {
    QTcpSocket* connectedSocket = m_server->nextPendingConnection();


    if (connectedSocket != nullptr) {
        if (m_host == nullptr) {
            m_host = connectedSocket;
        }

        QObject::connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
        QObject::connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);

        connectedSocket->flush();
    }
}

void Server::disconnected() {
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket *>(QObject::sender());

    qDebug() << "123\n";

    if (m_gameOn) {
        if (disconnectedSocket == m_host || disconnectedSocket == m_player1 || disconnectedSocket == m_player2) {
            qDebug() << "Nuke game?\n";
            nukeGame();
        }
        else {
            m_spectators.remove(disconnectedSocket);
            removeName(disconnectedSocket);
        }
    }
    else { // still in lobby
        if (disconnectedSocket == m_host) {
            qDebug() << "Nuke game - host????\n";
            nukeGame();
        }
        else {
            if (m_spectators.contains(disconnectedSocket)) {
                m_spectators.remove(disconnectedSocket);
            }
            else {
                if (disconnectedSocket == m_player1) {
                    m_player1 = nullptr;
                    processRemovePlayerCommand(m_host, m_clientNames[disconnectedSocket]);
                }
                else {
                    m_player2 = nullptr;
                }

                processRemovePlayerCommand(m_host, m_clientNames[disconnectedSocket]);
            }
            removeName(disconnectedSocket);
        }
    }

}

void Server::readMessage() {
    QTcpSocket* sourceSocket = dynamic_cast<QTcpSocket*>(sender());
    QString message = sourceSocket->readAll();

    if (message.startsWith(srvconst::serverCmdAddName)) {
        processAddNameCommand(sourceSocket, message.sliced(srvconst::serverCmdAddName.length()));
    }
    else if (message.startsWith(srvconst::serverCmdRemoveName)) {
        processRemoveNameCommand(sourceSocket, message.sliced(srvconst::serverCmdRemoveName.length()));
    }
    else if (message.startsWith(srvconst::serverCmdSelectPlayer)) {
        processSelectPlayerCommand(sourceSocket, message.sliced(srvconst::serverCmdSelectPlayer.length()));
    }
    else if (message.startsWith(srvconst::serverCmdRemovePlayer)) {
            processRemovePlayerCommand(sourceSocket, message.sliced(srvconst::serverCmdSelectPlayer.length()));
    }
    else if (message.startsWith(srvconst::serverCmdState)) {
        processStateCommand(sourceSocket, message.sliced(srvconst::serverCmdState.length()));
    }
    else if (message.startsWith(srvconst::serverCmdChat)) {
        processChatCommand(sourceSocket, message.sliced(srvconst::serverCmdChat.length()));
    }
    else if (message.startsWith(srvconst::serverCmdDoubling)) {
        processDoublingCommand(sourceSocket);
    }
    else if (message.startsWith(srvconst::serverCmdDice)) {
        processDiceCommand(sourceSocket, message);
    }
    else if (message.startsWith(srvconst::serverCmdGameStart)) {
        processGameStartCommand(sourceSocket);
    }
    else if (message.startsWith(srvconst::serverCmdGameEnd)) {
        processGameEndCommand(sourceSocket);
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
        if (m_player1 != src) {
            m_player1->write(message.toStdString().c_str());
            m_player1->flush();
        }
        if (m_player2 != src) {
            m_player2->write(message.toStdString().c_str());
            m_player2->flush();
        }
        for (auto spectator : m_spectators) {
            if (spectator != src) {
                spectator->write(message.toStdString().c_str());
                spectator->flush();
            }
        }
    }
}

void Server::processAddNameCommand(QTcpSocket* src, QString name) {
    for (auto c : name) {
        if (!c.isLetter() && !(c != '_')) {
            return; // handle in a better way
        }
    }

    // if (m_gameOn) {
    //     src->write(srvconst::serverCmdGameOn.toStdString().c_str());
    //     return;
    // }

    if (m_clientNames.find(src) == m_clientNames.end()) { // socket doesn't have a name registered
        QString finalName = name;

        if (m_namesCnt[name] == 0) {
            m_clientNames[src] = name;
            m_clientSockets[name] = src;
        }
        else {
            finalName = name + QString::number(m_namesCnt[name]);

            m_clientNames[src] = finalName;
            m_clientSockets[finalName] = src;
        }
        m_namesCnt[name]++;
        m_spectators.insert(src);

        // update local names list

        qDebug() << finalName << "\n";


        src->write((srvconst::serverCmdAddName + finalName).toStdString().c_str());

        auto names = m_namesCnt.keys();
        for (auto t : names) {
            if (t != finalName) {
                src->write((srvconst::serverCmdAddName + t).toStdString().c_str());
            }
        }

        broadcast(src, srvconst::serverCmdAddName + finalName);

        if (finalName == oppName) {
            processSelectPlayerCommand(m_host, finalName);
            processGameStartCommand(m_host);
        }
    }
}

void Server::processRemoveNameCommand(QTcpSocket* src, QString name) {

}

void Server::processSelectPlayerCommand(QTcpSocket* src, QString playerName) {
    if (src != m_host) {
        throw std::runtime_error("Only the host can choose opponent");
    }
    if (m_gameOn) {
        throw std::runtime_error("can't choose player after game has started");
    }


    if (m_clientSockets.find(playerName) != m_clientSockets.end()) {
        if (m_player1 != nullptr) {
            m_player1 = m_clientSockets[playerName];
        }
        else {
            m_player2 = m_clientSockets[playerName];
        }
        m_spectators.remove(m_clientSockets[playerName]);

        broadcast(nullptr, srvconst::serverCmdSelectPlayer + playerName, true);
    }
}

void Server::processRemovePlayerCommand(QTcpSocket* src, QString playerName) {
    if (src != m_host) {
        throw std::runtime_error("Only the host can choose opponent");
    }
    if (m_gameOn) {
        throw std::runtime_error("can't remove player after game has started");
    }


    if (m_clientSockets.find(playerName) != m_clientSockets.end()) {
        if (m_player1 == m_clientSockets[playerName]) {
            m_player1 = nullptr;
        }
        else {
            m_player2 = nullptr;
        }
        m_spectators.remove(m_clientSockets[playerName]);

        broadcast(nullptr, srvconst::serverCmdSelectPlayer + playerName, true);
    }
}

void Server::processStateCommand(QTcpSocket* src, QString state) {
    if (src != m_player1 && src != m_player2) {
        throw std::runtime_error("only players can change game state");
    }
    if (!m_gameOn) {
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

void Server::processDoublingCommand(QTcpSocket *src) {
    broadcast(src, (srvconst::serverCmdDoubling).toStdString().c_str(), true);
}

void Server::processDiceCommand(QTcpSocket *src, QString cmd) {
    broadcast(src, cmd.toStdString().c_str());
}


void Server::processRollCommand(QTcpSocket* src, QString roll) {
    broadcast(src, roll.toStdString().c_str(), true);
}

void Server::processTurnCommand(QTcpSocket* src, QString turn) {
    broadcast(src, turn.toStdString().c_str(), true);
}


void Server::processGameStartCommand(QTcpSocket *src) {
    m_gameOn = true;
    broadcast(nullptr, (srvconst::serverCmdGameStart).toStdString().c_str(), true);
}

void Server::processGameEndCommand(QTcpSocket *src) {
    m_gameOn = false;
    broadcast(src, (srvconst::serverCmdGameEnd).toStdString().c_str(), true);

    nukeGame();
}

void Server::nukeGame() {
    qDebug() << "Reached\n";
    // processRemoveNameCommand(m_host, m_clientNames[m_player1]);
    processRemoveNameCommand(m_host, m_clientNames[m_player2]);
    for (auto spectator : m_spectators)
        processRemoveNameCommand(m_host, m_clientNames[spectator]);
    // m_player1 = nullptr; // this will always be host
    m_player2 = nullptr;
    m_spectators.clear();
    m_clientNames.clear();
    m_clientSockets.clear();
    // throw std::runtime_error("handle differently?");
}

void Server::removeName(QTcpSocket *disconnectedSocket) {
    broadcast(disconnectedSocket, (srvconst::serverCmdRemoveName + m_clientNames[disconnectedSocket]));

    m_clientSockets.erase(m_clientSockets.find(m_clientNames[disconnectedSocket]));
    m_clientNames.erase(m_clientNames.find(disconnectedSocket));
}

