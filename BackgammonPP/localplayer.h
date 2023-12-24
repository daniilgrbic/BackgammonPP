#pragma once

#include <QObject>
#include "playableplayer.h"

class LocalPlayer : public PlayablePlayer
{
    Q_OBJECT
public:
    explicit LocalPlayer(QObject *parent = nullptr);

public slots:
    void askedToDouble(BoardState* state) override;
    void receivedState(BoardState* state) override;
    void chooseMove(std::vector<Turn> &legalMoves) override;
    void acceptDice(int diceValue) override;
};
