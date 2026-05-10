#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Gun.h"
#include "Collidable.h"
#include "Player.h"

class RocketMob : public Collidable
{
public:
    RocketMob(const sf::Vector2f &spwnPos = {500.f, 500.f});

    sf::FloatRect GetBounds() const override;
    const sf::Sprite &GetSprite() const override;

    void TakeDamage(int sourceDamage);
    bool IsAlive() const { return alive; }

    void Load();
    void Update(float deltaTime, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture rocketMobTexture;
    sf::Sprite rocketMobSprite;

    sf::Vector2f currentDestination{1.f, 1.f};
    sf::Vector2f spawnLocation;

    // WEAPON AND AMMO STATS
    float launcherVelocity = 60.f; 
    float launcherFireRate = 0.5f;
    int launcherDamage = 50;

    float rocketVelocity = 15.f;
    int rocketDamage = 30;          

    Gun<RocketBullet> rocketLauncher{launcherVelocity, launcherFireRate, launcherDamage};

    // STATS
    int health = 100;
    int defense = 20;     
    float speed = 20.f;              
    bool alive = true;
    float moveCooldown = 2.f;

    float moveTimer;
    bool moving = false;
};