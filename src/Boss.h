#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Gun.h"
#include "RocketGun.h"
#include "FlameGun.h"
#include "Collidable.h"
#include "Player.h"

#define TILE_WIDTH_BOSS 250
#define TILE_HEIGHT_BOSS 166

class Player; // resolving circular dependency
class Officer;
class Tank;

class Boss : public Collidable
{
public:
    Boss(const sf::Vector2f &spwnPos = {900.f, 900.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);
    bool IsAlive() const;

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture bossTexture;
    sf::Sprite bossSprite;

    sf::Vector2f currentDestination{1.f, 1.f};
    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float gunVelocity = 200.f;
    float gunFireRate = 4.f;
    int gunDamage = 40;

    float turretVelocity = 400.f;
    float turretRate = 8.f;
    int turretDamage = 30;

    float rocketLauncherVel = 75.f;
    float rocketLauncherRate = 2.f;
    int rocketLauncherDmg = 100;

    float throwerVel = 40.f;
    float throwerRate = 300.f;
    int throwerDmg = 10;
    int throwerInty = 5;
    float throwerSpread = 0.75f;

    float bulletVelocity = 50.f;
    int bulletDamage = 10;

    int rocketDamage = 50;
    float rocketVelocity = 25.f;
    float rocketTimeLimit = 4.5f;
    float rocketBlastRad = 25.f;
    float rocketBlastTime = 1.f;

    int flameDmg = 10;
    float flameVel = 10.f;
    float flameMaxDist = 200.f;

    Gun gun{gunVelocity, gunFireRate, gunDamage};
    Gun turret{turretVelocity, turretRate, turretDamage};
    RocketGun rocketLauncher{rocketLauncherVel, rocketLauncherRate, rocketLauncherDmg};
    FlameGun flameThrower{throwerVel, throwerRate, throwerDmg, throwerInty, throwerSpread};

    // STATS
    int maxHealth = 5000;
    int defense = 50;
    float speed = 40.f;
    bool alive = true;
    float moveCooldown = 5.f;
    float immunityTime = 0.5f;

    int health;
    float moveTimer;
    float immunityTimer;
    bool moving = false;
    int phase = 0; // 0 only shoots, 1 shoots and fires rockets, 2 shoots fires rockets and spawns mobs, 3 speed increases and shoots flamethrower
};