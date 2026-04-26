#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map();
    ~Map();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);

private:
    const sf::Vector2f tileSize{16, 16};
    const sf::Vector2f worldSizeTiles{100, 100};

    sf::Texture worldTexture;
    sf::VertexArray backgroundVertices;
    std::vector<int> backgroundTexTiles;
    // midground tiles
    // foreground tiles
};