#pragma once

#include "boardstate.h"
#include "turn.h"

#include <optional>

class TurnTrie {
public:
    TurnTrie(const std::vector<Turn>& turns, const BoardState& board);

    ~TurnTrie();

    TurnTrie(const TurnTrie&) = delete;
    TurnTrie(TurnTrie&&) = delete;
    TurnTrie& operator =(const TurnTrie&) = delete;
    TurnTrie& operator =(TurnTrie&&) = delete;

    BoardState board() const;

    bool hasNextMove(const Move& move) const;
    std::vector<Move> nextMoves() const;

    bool isFinishedTurn() const;
    Turn getTurn() const;

    void playMove(const Move& move);

    bool canUndo();
    void undoMove();

private:
    struct TurnNode {
        TurnNode(TurnNode* parent = nullptr)
            : parent { parent }
        {}

        ~TurnNode() {
            for (const auto& [move, child] : children)
                delete child;
        }

        TurnNode(const TurnNode&) = delete;
        TurnNode(TurnNode&&) = delete;
        TurnNode& operator =(const TurnNode&) = delete;
        TurnNode& operator =(TurnNode&&) = delete;

        TurnNode* parent { nullptr };
        std::unordered_map<Move, TurnNode*> children {};
        std::optional<BoardState> board { std::nullopt };
        std::optional<Turn> turn { std::nullopt };
    };

    TurnNode *m_root;
    TurnNode *m_currentNode;
};
