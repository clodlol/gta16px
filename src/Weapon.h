#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"
#include "Collidable.h"

struct Bullet : public Collidable
{
public:
    sf::FloatRect GetBounds() override
    {
        return sprite.getGlobalBounds();
    }

    Bullet(const sf::Texture &tex) : sprite(tex) {}

    sf::Sprite sprite;

    std::string source;
    sf::Vector2f origin;
    sf::Angle direction;

    int damage;
};

class Weapon
{
public:
    Weapon(float vel, float rate, int dmg);

    void Fire(const sf::Vector2f &origin, const sf::Angle &direction, const std::string &source);
    void StopFire();
    const Bullet &GetCurrentBullet();

    void Load();
    void Update(float deltaTime, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture bulletTexture;
    Bullet currentBullet;
    std::vector<Bullet> bullets;

    float bulletVelocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 1;

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};