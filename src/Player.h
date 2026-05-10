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
#include "FlameGun.h"

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
    float throwerVelocity = 25.f;
    float throwerFireRate = 500.f;
    int throwerIntensity = 10;
    float throwerSpread = 0.8;
    int throwerDamage = 1;

    float flameVeocity = 25.f;
    int flameDamage = 1;
    float flameMaxDistance = 50.f;

    // Gun<Bullet> gun{gunVelocity, gunFireRate, gunDamage};
    // Melee sword{10.f, 1.f, 20};
    FlameGun flamethrower{throwerVelocity, throwerFireRate, throwerDamage, throwerIntensity, throwerSpread};

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