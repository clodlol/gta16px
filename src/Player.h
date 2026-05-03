#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"

class Player
{
public:
    Player();

    void Initialize();
    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    int health;
    float speed;
};