#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Collidable.h"
#include "Player.h"

class Officer : public Collidable
{
public:
    Officer();

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture officerTexture;
    sf::Sprite officerSprite;

    sf::Vector2f currentDestination;

    Weapon<Bullet> pistol{125.f, 2.f, 20};

    int health = 50;
    int defense = 5;
    float speed = 40.f;
    bool alive = true;

    float moveTimer = 5.f;
    bool moving = false;
};