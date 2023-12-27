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

    void playMove(const Move& move);

    bool canUndo();
    void undoMove();

private:
    struct TurnNode {
        TurnNode(TurnNode* parent = nullptr)
            : parent { parent }
        {}

        TurnNode(const TurnNode& node)
            : parent { node.parent }
            , board { node.board }
            , turn { node.turn }
        {
            for (const auto& [move, child] : node.children) {
                auto* childCpy = new TurnNode { *child };
                childCpy->parent = this;
                children[move] = childCpy;
            }
        }

        TurnNode* parent { nullptr };
        std::unordered_map<Move, TurnNode*> children {};
        std::optional<BoardState> board { std::nullopt };
        std::optional<Turn> turn { std::nullopt };
    };

    TurnNode *m_root;
    TurnNode *m_currentNode;

    void generateLinkedMoves(TurnNode* root);
};
