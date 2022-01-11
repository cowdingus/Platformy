#include "Engine/Core/JSONSerializer.hpp"

#include "Engine/Core/World.hpp"
#include "Engine/Core/BlockDescriptor.hpp"

#include <vector>

void to_json(json& j, const World& w)
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

void from_json(const json& j, World& w)
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

void to_json(json& j, const BlockDescriptor& t) {
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

void from_json(const json& j, BlockDescriptor& t) {
    j.at("id").get_to(t.id);
    j.at("name").get_to(t.name);
    j.at("atlasIndex").get_to(t.atlasIndex);

    t.textureRect.left = j.at("textureRect").at(0).get<int>();
    t.textureRect.top = j.at("textureRect").at(1).get<int>();
    t.textureRect.width = j.at("textureRect").at(2).get<int>();
    t.textureRect.height = j.at("textureRect").at(3).get<int>();
}
