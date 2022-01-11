#pragma once

#include "json.hpp"

#include "Engine/Core/TextureAtlas.hpp"
#include "Engine/Core/BlockDescriptor.hpp"
#include "Engine/Core/Grid2D.hpp"

#include <vector>

using BlockId = uint32_t;
using WorldMap = Grid2D<BlockId>;

/* S: World */
struct World {
    std::vector<TextureAtlas> textureAtlases;
    std::vector<BlockDescriptor> blockDescriptors;
    WorldMap map;
};
/* C: World on World.cpp */
