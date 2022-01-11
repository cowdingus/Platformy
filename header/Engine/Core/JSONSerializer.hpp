#pragma once

#include "json.hpp"
using nlohmann::json;

class World;
class BlockDescriptor;

void to_json(json& j, const World& w);
void from_json(const json& j, World& w);

void to_json(json& j, const BlockDescriptor& t);
void from_json(const json& j, BlockDescriptor& t);
