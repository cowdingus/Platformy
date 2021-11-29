#pragma once

#include "json.hpp"

#include "Data/TextureAtlas.hpp"
#include "Data/BlockDescriptor.hpp"
#include "Data/Grid2D.hpp"

#include <vector>
#include <exception>

using json = nlohmann::json;

using BlockId = uint32_t;
using WorldMap = Grid2D<BlockId>;

/* S: World */
struct World {
    std::vector<TextureAtlas> textureAtlases;
    std::vector<BlockDescriptor> blockDescriptors;
    WorldMap map;
};

inline void to_json(json& j, const World& w)
{
    std::vector<std::string> atlasPaths;

    for (const auto& atlas : w.textureAtlases) {
        atlasPaths.push_back(atlas.path);
    }

    j = json{
        {"map", w.map},
        {"textureAtlases", atlasPaths},
        {"blockDescriptions", w.blockDescriptors},
    };
}

inline void from_json(const json& j, World& w)
{
    j.at("map").get_to(w.map);
    j.at("blockDescriptions").get_to(w.blockDescriptors);

    auto atlasPaths = j.at("textureAtlases").get<std::vector<std::string>>();

    for (const auto& atlasPath : atlasPaths)
    {
        w.textureAtlases.push_back({atlasPath, {}});
        if (!w.textureAtlases.back().texture.loadFromFile(atlasPath))
        {
            throw std::runtime_error("Can't load texture specified by json textureAtlases");
        }
    }
}
/* C: World on World.cpp */
