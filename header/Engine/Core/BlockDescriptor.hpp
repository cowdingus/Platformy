#pragma once

#include "json.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <string>

using nlohmann::json;

/* S: BlockDescriptor */
struct BlockDescriptor {
    uint32_t id {};
    std::string name {};
    uint32_t atlasIndex {};
    sf::IntRect textureRect {};
};
/* E: BlockDescriptor */
