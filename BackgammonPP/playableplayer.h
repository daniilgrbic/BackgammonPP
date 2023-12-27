#pragma once

#include <QObject>
#include "player.h"

class PlayablePlayer : public Player
{
    Q_OBJECT
public:
    explicit PlayablePlayer(QObject *parent = nullptr);

signals:
    void showDoubleButton();
    void showDoubleQuestion();
    void throwDice();
    void showState(BoardState* state);

public slots:
    virtual void acceptDice(int diceValue) = 0;
};
