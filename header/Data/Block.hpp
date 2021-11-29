#pragma once

#include "json.hpp"

#include <SFML/System/Vector2.hpp>

using nlohmann::json;

/* S: Block */
struct Block {
    uint32_t id {};
    sf::Vector2i position {};
};

inline void to_json(json& j, const Block& t)
{
    j = json{t.id, t.position.x, t.position.y};
}

inline void from_json(const json& j, Block& t)
{
    j.at(0).get_to(t.id);

    int x = j.at(1).get<int>();
    int y = j.at(2).get<int>();
    t.position = {x,y};
}
/* E: Block */
