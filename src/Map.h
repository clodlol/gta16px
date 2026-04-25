#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Map
{
private:
    const int tileWidth, tileHeight = 16;
    const int tilesPerRow = 37;
    const int tilesPerColumn = 28;

    const int worldWidthTiles = 100;
    const int worldHeightTiles = 100;

    sf::Texture worldTexture;
    sf::VertexArray backgroundTiles;
    // midground tiles
    // foreground tiles

public:
    Map();
    ~Map();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);
};