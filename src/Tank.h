#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Gun.h"
#include "Collidable.h"
#include "Player.h"

class Player;

class Tank : public Collidable
{
public:
    Tank(const sf::Vector2f &spwnPos = {900.f, 900.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);
    bool IsAlive() const;

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;

    sf::Vector2f currentDestination{1.f, 1.f};
    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float cannonVelocity = 200.f;
    float cannonFireRate = 4.f; // bullets per second
    int cannonDamage = 40;

    float bulletVelocity = 50.f;
    int bulletDamage = 10;

    Gun heavyCannon{cannonVelocity, cannonFireRate, cannonDamage};

    // STATS
    int maxHealth = 300;
    int defense = 30;
    float speed = 15.f;
    float aggro = 0.5f; // Aggro is a float between 0 and 1 and determines how close(aggro high) or how far(aggro low) the tank tends to be to the player
    bool alive = true;
    float moveCooldown = 2.f;
    float immunityTime = 0.5f;

    int health = 300;
    float moveTimer;
    float immunityTimer;
    bool moving = false;
};