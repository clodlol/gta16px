#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"
#include "Gun.h"
#include "Collidable.h"

class Player : public Collidable
{
public:
    Player(const sf::Vector2f &spwnPos = {1000.f, 1000.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float gunVelocity = 150.f;
    float gunFireRate = 5.f;
    int gunDamage = 20;

    float bulletVelocity = 100.f;
    int bulletDamage = 20;

    Gun<Bullet> gun{gunVelocity, gunFireRate, gunDamage};

    // PLAYER STATS
    int health = 100;
    int defense = 50;
    float speed = 50.f;
    float immunityTime = 2.f;
    float respawnTime = 5.f;

    float immunityTimer;
    float deathTimer;
    bool alive = true;
};