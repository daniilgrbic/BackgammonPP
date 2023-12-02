#pragma once

#include "playercolor.h"

#include <string>
#include <optional>
#include <map>

struct GameResult {
    const std::optional<PlayerColor> winner;
    const std::map <PlayerColor, int> points;
    const std::string victoryTypeName;
};
