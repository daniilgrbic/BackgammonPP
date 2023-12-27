#include "turntrie.h"

#include <queue>

TurnTrie::TurnTrie(const std::vector<Turn>& turns, const BoardState& board)
    : m_root { new TurnNode {} }, m_currentNode { m_root }
{
    m_root->board = board;

    for (const auto& turn : turns) {
        auto* currentNode = m_root;
        for (const auto& move : turn.m_moves) {
            if (!currentNode->children.contains(move)) {
                auto* node = new TurnNode(currentNode);
                currentNode->children[move] = node;
            }
            currentNode = currentNode->children[move];
        }
        currentNode->turn = turn;
    }

    generateLinkedMoves(m_root);
}

TurnTrie::~TurnTrie() {
    for (auto& child : m_root->children) {
        delete child.second;
    }
    delete m_root;
}

BoardState TurnTrie::board() const {
    return m_currentNode->board.value();
}

bool TurnTrie::hasNextMove(const Move& move) const {
    return m_currentNode->children.contains(move);
}

std::vector<Move> TurnTrie::nextMoves() const {
    std::vector<Move> moves;
    std::transform(m_currentNode->children.begin(), m_currentNode->children.end(), std::back_inserter(moves),
                   [](const auto& child) { return child.first; });
    return moves;
}

bool TurnTrie::isFinishedTurn() const {
    return m_currentNode->turn.has_value();
}

Turn TurnTrie::getTurn() const {
    assert(isFinishedTurn());
    return m_currentNode->turn.value();
}

void TurnTrie::playMove(const Move& move) {
    assert(hasNextMove(move));

    auto* nextNode = m_currentNode->children[move];
    if (!nextNode->board)
        nextNode->board = m_currentNode->board.value().getNextState(move);

    m_currentNode = nextNode;
}

bool TurnTrie::canUndo(){
    return m_currentNode->parent;
}

void TurnTrie::undoMove() {
    if (m_currentNode->parent)
        m_currentNode = m_currentNode->parent;
}

void TurnTrie::generateLinkedMoves(TurnNode* root) {
    std::queue<std::pair<Move, TurnNode*>> children;

    for (const auto& child : root->children)
        children.push(child);

    while (!children.empty()) {
        const auto& [childMove, child] = children.front();
        children.pop();
        generateLinkedMoves(child);
        for (const auto& [gcMove, grandchild] : child->children) {
            if (gcMove.m_from == childMove.m_to) {
                auto move = childMove;
                move.m_to = gcMove.m_to;
                move.m_hittedPoints.insert(
                    move.m_hittedPoints.end(),
                    gcMove.m_hittedPoints.begin(),
                    gcMove.m_hittedPoints.end()
                );
                auto* node = new TurnNode { *grandchild };
                node->parent = root;

                root->children[move] = node;
                children.emplace(std::make_pair(std::move(move), node));
            }
        }
    }
}
