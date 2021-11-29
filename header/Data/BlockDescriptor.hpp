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

inline void to_json(json& j, const BlockDescriptor& t) {
    j = json{
        {"id", t.id},
        {"atlasIndex", t.atlasIndex},
        {"name", t.name},
        {"textureRect", {
            t.textureRect.left,
            t.textureRect.top,
            t.textureRect.width,
            t.textureRect.height
        }}
    };
}

inline void from_json(const json& j, BlockDescriptor& t) {
    j.at("id").get_to(t.id);
    j.at("name").get_to(t.name);
    j.at("atlasIndex").get_to(t.atlasIndex);

    t.textureRect.left = j.at("textureRect").at(0).get<int>();
    t.textureRect.top = j.at("textureRect").at(1).get<int>();
    t.textureRect.width = j.at("textureRect").at(2).get<int>();
    t.textureRect.height = j.at("textureRect").at(3).get<int>();
}
/* E: BlockDescriptor */
