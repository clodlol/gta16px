#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"
#include "Weapon.h"
#include "Collidable.h"

class Player : public Collidable
{
public:
    Player();

    sf::FloatRect GetBounds() override;

    void TakeDamage(int sourceDamage);

    void Initialize();
    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    Weapon gun{0.f, 0.f, 0};

    int health;
    int defense;
    float speed;
};