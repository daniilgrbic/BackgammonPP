#pragma once

#include "boardchecker.h"
#include "engine/core/specialposition.h"
#include "engine/core/playercolor.h"
#include <QVector>

#include <variant>
#include <functional>

using HolderType = std::variant<int, SpecialPosition>;
class BoardChecker;

class CheckerHolder
{
public:
    CheckerHolder();
    virtual void addChecker(BoardChecker* checker);
    virtual void removeChecker(BoardChecker* checker);
    virtual void updateCheckerPos() = 0;
    void enableCheckers(const PlayerColor& color);
    void setAllowDropoff(bool allowDropoff);
    bool canDropoff();
    virtual void updateHighlighting() = 0;


    HolderType m_type;
    QVector<BoardChecker*> m_checkers;

private:
    bool m_allowDropoff = false;
};
