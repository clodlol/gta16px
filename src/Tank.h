#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Collidable.h"
#include "Player.h"

class Tank : public Collidable
{
public:
    Tank();

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;

    sf::Vector2f currentDestination;

    Weapon<Bullet> heavyCannon{250.f, 4.f, 50};

    int health = 300;
    int defense = 30;
    float speed = 20.f;
    bool alive = true;

    float moveTimer = 3.f;
    bool moving = false;
};