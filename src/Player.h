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
#include "RocketGun.h"

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
    sf::Font font;
    sf::Sprite playerSprite;

    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float launcherVelocity = 75.f;
    float launcherFireRate = 2.f;
    int launcherDamage = 300;

    float gunVelocity = 200.f;
    float gunFireRate = 4.f;
    int gunDamage = 70;

    int bulletDamage = 20;
    float bulletVelocity = 50.f;

    float swordSpeed = 10.f;
    int swordDamage = 200;

    int rocketDamage = 50;
    float rocketVelocity = 25.f;
    float rocketTimeLimit = 2.5f;
    float rocketBlastRad = 50.f;
    float rocketBlastTime = 1.f;

    float throwerVelocity = 40.f;
    float throwerFireRate = 500.f;
    int throwerDamage = 10;
    int throwerIntensity = 5;
    float throwerSpread = 0.75;

    int flameDamage = 5;
    float flameVelocity = 20.f;
    float flameMaxDistance = 150.f;

    Gun gun{gunVelocity, gunFireRate, gunDamage};
    Melee sword{swordSpeed, swordDamage};
    FlameGun flamethrower{throwerVelocity, throwerFireRate, throwerDamage, throwerIntensity, throwerSpread};
    RocketGun rocketLauncher{launcherVelocity, launcherFireRate, launcherDamage};

    // PLAYER STATS
    int maxHealth = 1000;
    int defense = 50;
    float speed = 150.f;
    float immunityTime = 2.f;
    float respawnTime = 5.f;

    int health = 1000;
    float immunityTimer;
    float deathTimer;
    bool alive = true;
    int hero = 0; // 0 for gun, 1 for melee, 2 for flamethrower, 3 for rocketgun
};