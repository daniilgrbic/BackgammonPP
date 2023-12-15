#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <system_error>


class Player : public QObject {
    Q_OBJECT
public:
    Player(QObject* parent = nullptr);
    ~Player();

signals:
    void displayNewTurn(Turn turn);
    void requestLegalMoves();

public slots:
    virtual void acceptNewTurn(QString turn);
    virtual void chooseMove(std::vector<Turns>);

private:
    Client* m_client;
};

#endif // PLAYER_H
