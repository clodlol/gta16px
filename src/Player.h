#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"
#include "Gun.h"
#include "Collidable.h"
#include "MobSpawner.h"
#include "Officer.h"
#include "Tank.h"
#include "Melee.h"

class MobSpawner; // resolving circular dependency
class Officer;
class Tank;
class Melee;

class Player : public Collidable
{
public:
    Player(const sf::Vector2f &spwnPos = {1000.f, 1000.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);

    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera, MobSpawner &spawner);
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

    // Gun<Bullet> gun{gunVelocity, gunFireRate, gunDamage};
    Melee sword{1.f, 20};

    // PLAYER STATS
    int health = 50;
    int defense = 100;
    float speed = 150.f;
    float immunityTime = 2.f;
    float respawnTime = 5.f;

    float immunityTimer;
    float deathTimer;
    bool alive = true;
};