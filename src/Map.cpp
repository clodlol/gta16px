#pragma once
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Map.h"

std::vector<int> &parseTextureTiles(std::filesystem::path path, std::vector<int> &v)
{
    std::ifstream fin(path);
    int val;

    while (fin >> val)
    {
        v.push_back(val);

        if (fin.peek() == ',')
            fin.ignore();
    }

    return v;
}

Map::Map() : worldTexture{}, backgroundVertices{sf::PrimitiveType::Triangles, worldSizeTiles.x * worldSizeTiles.y * 6}, backgroundTexTiles{(int)(worldSizeTiles.x * worldSizeTiles.y)}
{
    parseTextureTiles("../world/baselayer.csv", backgroundTexTiles);

    std::cout << "Found " << backgroundTexTiles.size() << " tiles in background texture map file.\n";
}

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

    for (int y = 0; y < worldSizeTiles.y; ++y)
    {
        for (int x = 0; x < worldSizeTiles.x; ++x)
        {
            int tileNumber = backgroundTexTiles[x + y * worldSizeTiles.x];

            int tu = tileNumber % (int(worldTexture.getSize().x / tileSize.x));
            int tv = tileNumber / (worldTexture.getSize().x / tileSize.x);

            sf::Vertex *v = &backgroundVertices[(x + y * worldSizeTiles.x) * 6];

            float left = x * (float)tileSize.x;
            float right = (x + 1) * (float)tileSize.x;
            float top = y * (float)tileSize.y;
            float bottom = (y + 1) * (float)tileSize.y;

            float tLeft = tu * (float)tileSize.x;
            float tRight = (tu + 1) * (float)tileSize.x;
            float tTop = tv * (float)tileSize.y;
            float tBottom = (tv + 1) * (float)tileSize.y;

            v[0].position = {left, top};
            v[1].position = {right, top};
            v[2].position = {right, bottom};
            v[3].position = {right, bottom};
            v[4].position = {left, bottom};
            v[5].position = {left, top};

            v[0].texCoords = {tLeft, tTop};
            v[1].texCoords = {tRight, tTop};
            v[2].texCoords = {tRight, tBottom};
            v[3].texCoords = {tRight, tBottom};
            v[4].texCoords = {tLeft, tBottom};
            v[5].texCoords = {tLeft, tTop};
        }
    }
}

void Map::Update(float deltaTime)
{
}

void Map::Draw(sf::RenderWindow &window)
{
    window.draw(backgroundVertices, &worldTexture);
}