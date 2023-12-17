#include "move.h"
#include "boardstate.h"

Move::Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to)
    : m_player { player }, m_from { from }, m_to { to }
{}

Move Move::mirror() const {
    std::variant<int, SpecialPosition> from;
    if (const int* fromPos = std::get_if<int>(&m_from)) {
        from = NUMBER_OF_POINTS + 1 - *fromPos;
    } else {
        from = m_from;
    }

    std::variant<int, SpecialPosition> to;
    if (const int* toPos = std::get_if<int>(&m_to)) {
        to = NUMBER_OF_POINTS + 1 - *toPos;
    } else {
        to = m_to;
    }

    return { m_player, from, to };
}
