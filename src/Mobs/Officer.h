#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Collidable.h"

class Officer : public Collidable
{
public:
    Officer();

    sf::FloatRect GetBounds() override;

    void TakeDamage(int sourceDamage);
    void Kill();

    void Load();
    void Update(float deltaTime, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture officerTexture;
    sf::Sprite officerSprite;

    Weapon pistol{75.f, 2.f, 20};

    int health = 50;
    int defense = 5;
    float speed = 25.f;
    bool alive = true;

    float moveCooldown = 5.f;
};