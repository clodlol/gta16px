#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Gun.h"
#include "Collidable.h"
#include "Player.h"

class Officer : public Collidable
{
public:
    Officer(const sf::Vector2f &spwnPos = {900.f, 900.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture officerTexture;
    sf::Sprite officerSprite;

    sf::Vector2f currentDestination{1.f, 1.f};
    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float pistolVelocity = 125.f;
    float pistolFireRate = 2.f;
    int pistolDamage = 20;

    float bulletVelocity = 25.f;
    int bulletDamage = 10;

    Gun<Bullet> pistol{pistolVelocity, pistolFireRate, pistolDamage};

    // STATS
    int health = 50;
    int defense = 5;
    float speed = 40.f;
    bool alive = true;
    float moveCooldown = 5.f;

    float moveTimer;
    bool moving = false;
};