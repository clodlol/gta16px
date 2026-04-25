#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Map.h"

Map::Map() : tileWidth{16}, tileHeight{16}, tilesPerRow{37}, tilesPerColumn{28}, worldWidthTiles{100}, worldHeightTiles{100}, worldTexture{}, backgroundTiles{sf::PrimitiveType::TriangleStrip, worldWidthTiles * worldHeightTiles * 6} {}

Map::~Map()
{
}

void Map::Initialize()
{
}

void Map::Load()
{
    if (!worldTexture.loadFromFile("../assets/tilesheets/map_tilesheet.png"))
    {
        std::cout << "Failed to load world tilesheet\n";
        return;
    }

    for (int y = 0; y < worldHeightTiles; ++y)
    {
        for (int x = 0; x < worldWidthTiles; ++x)
        {
            int index = (x + y * worldWidthTiles) * 6;
            sf::Vertex *v = &backgroundTiles[index];

            float left = x * (float)tileWidth;
            float right = (x + 1) * (float)tileWidth;
            float top = y * (float)tileHeight;
            float bottom = (y + 1) * (float)tileHeight;

            float tL = 0.f, tR = 16.f, tT = 0.f, tB = 16.f;

            v[0].position = {left, top};
            v[1].position = {right, top};
            v[2].position = {right, bottom};
            v[3].position = {right, bottom};
            v[4].position = {left, bottom};
            v[5].position = {left, top};

            v[0].texCoords = {tL, tT};
            v[1].texCoords = {tR, tT};
            v[2].texCoords = {tR, tB};
            v[3].texCoords = {tR, tB};
            v[4].texCoords = {tL, tB};
            v[5].texCoords = {tL, tT};
        }
    }
}

void Map::Update(float deltaTime)
{
}

void Map::Draw(sf::RenderWindow &window)
{
    window.draw(backgroundTiles, &worldTexture);
}