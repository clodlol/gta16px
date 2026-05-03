#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();
    ~Player();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
};