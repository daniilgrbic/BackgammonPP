#pragma once

#include "boardstate.h"
#include "turn.h"

#include <optional>

class TurnTrie {
public:
    TurnTrie(const std::vector<Turn>& turns, const BoardState& board);

    ~TurnTrie();

    BoardState board() const;

    bool hasNextMove(const Move& move) const;
    std::vector<Move> nextMoves() const;

    bool isFinishedTurn() const;
    Turn getTurn() const;

    void playMove(Move& move);
    void undoMove();

private:
    struct TurnNode {
        TurnNode(TurnNode* parent = nullptr)
            : parent { parent }
        {}

        TurnNode* parent { nullptr };
        std::unordered_map<Move, TurnNode*> children {};
        std::optional<BoardState> board { std::nullopt };
        std::optional<Turn> turn { std::nullopt };
    };

    TurnNode *m_root;
    TurnNode *m_currentNode;
};
