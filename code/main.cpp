#include "Data/Block.hpp"
#include "Data/BlockDescriptor.hpp"
#include "Data/TextureAtlas.hpp"
#include "Data/World.hpp"

#include "json.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

using nlohmann::json;

namespace cnst
{
    const sf::Vector2f tileSize {32.f, 32.f};
}

World loadWorld()
{
    const std::string worldDataFilepath = "data/world.json";

    json worldJson;

    std::ifstream worldData {worldDataFilepath};
    worldData >> worldJson;

    World world = worldJson;

    return world;
}

void drawWorld(const World& world, sf::RenderTarget& target)
{
    const sf::Vector2f tileSize = cnst::tileSize;

    std::vector<sf::VertexArray> batches;
    batches.resize(world.textureAtlases.size());

    for (int y = 0; y < world.map.getHeight(); ++y)
    {
        for (int x = 0; x < world.map.getWidth(); ++x)
        {
            BlockId id = world.map.get(x, y);

            const auto& blockDesc = world.blockDescriptors[id];
            const auto& textureRect = blockDesc.textureRect;

            auto& batch = batches.at(blockDesc.atlasIndex);

            batch.append(sf::Vertex(
                {x * tileSize.x, y * tileSize.y},
                sf::Color::White,
                sf::Vector2f(textureRect.left, textureRect.top)
            ));
            batch.append(sf::Vertex(
                {x * tileSize.x, (y + 1) * tileSize.y},
                sf::Color::White,
                sf::Vector2f(textureRect.left, textureRect.top + textureRect.height)
            ));
            batch.append(sf::Vertex(
                {(x + 1) * tileSize.x, (y + 1) * tileSize.y},
                sf::Color::White,
                sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height)
            ));
            batch.append(sf::Vertex(
                {(x + 1) * tileSize.x, y * tileSize.y},
                sf::Color::White,
                sf::Vector2f(textureRect.left + textureRect.width, textureRect.top)
            ));
        }
    }

    for (std::size_t x = 0; x < world.textureAtlases.size(); ++x)
    {
        auto& batch = batches.at(x);

        batch.setPrimitiveType(sf::Quads);

        sf::RenderStates states;
        states.texture = &world.textureAtlases.at(x).texture;

        target.draw(batch, states);
    }
}

void placeBlock(World& world, BlockId id, size_t x, size_t y)
{
    world.map.set(x, y, id);
}

std::tuple<size_t, size_t> toBlockPosition(sf::Vector2f pos)
{
    assert(pos.x > 0 && pos.y > 0);

    float x_f = pos.x / cnst::tileSize.x;
    float y_f = pos.y / cnst::tileSize.y;

    return {std::floor(x_f), std::floor(y_f)};
}

int main()
{
    World world = loadWorld();

    sf::Vector2i mouseWPos {};
    sf::Vector2f mouseVPos {};

    bool rmbPressed {};
    bool lmbPressed {};

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    mouseWPos = {event.mouseMove.x, event.mouseMove.y};
                    mouseVPos = window.mapPixelToCoords(mouseWPos);
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) lmbPressed = true;
                    else if (event.mouseButton.button == sf::Mouse::Right) rmbPressed = true;

                    mouseWPos = {event.mouseButton.x, event.mouseButton.y};
                    mouseVPos = window.mapPixelToCoords(mouseWPos);
                    break;
                default: break;
            }
        }

        if (lmbPressed)
        {
            auto [x, y] = toBlockPosition(mouseVPos);
            std::cout << x << "," << y << std::endl;
            placeBlock(world, 0, x, y);
            lmbPressed = false;
        }

        window.clear();
        drawWorld(world, window);
        window.display();
    }
    return 0;
}
