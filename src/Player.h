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

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    Weapon<Bullet> gun{150.f, 5.f, 20};

    int health = 100;
    int defense = 10;
    float speed = 50.f;
    float immunityTime = 2.f;

    float immunityTimer = immunityTime;
    bool alive = true;
};