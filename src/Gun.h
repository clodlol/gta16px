#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <type_traits>

#include "InputManager.h"
#include "Collidable.h"

#define TILE_SIZE_BULLET 225

#include <SFML/Graphics.hpp>

class Bullet : public Collidable
{
public:
    Bullet(const sf::Texture &tex, const sf::Vector2f &ogn, const sf::Vector2f &dir, int dmg, float vel) : sprite(tex), damage(dmg), velocity(vel), origin(ogn), direction(dir)
    {
        sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_BULLET, TILE_SIZE_BULLET}));
        sprite.setOrigin({TILE_SIZE_BULLET / 2, TILE_SIZE_BULLET / 2});
        sprite.scale({8.f / TILE_SIZE_BULLET, 8.f / TILE_SIZE_BULLET});
        sprite.rotate(direction.angle());
        sprite.setPosition(origin);
    }

    sf::FloatRect GetBounds() const override { return sprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return sprite; }

    const sf::Vector2f &GetOrigin() const { return origin; }
    const sf::Vector2f &GetDirection() const { return direction; }

    int GetDamage() const { return damage; }
    float GetVelocity() const { return velocity; }

    void Erase() { expired = true; }

    bool IsExpired()
    {
        return expired;
    }

    friend class Gun;

private:
    sf::Sprite sprite;

    sf::Vector2f origin{1.f, 1.f};
    sf::Vector2f direction{1.f, 1.f};

    bool expired = false;

    int damage;
    float velocity;
};

class Gun
{

public:
    Gun(float vel, float rate, int dmg) : velocity(vel), fireRate(rate), damage(dmg)
    {
        cooldownTimer = 1.f / fireRate;
    }

    void Fire(const Bullet &projectile)
    {
        currentProjectile = projectile;
        firing = true;
    }

    void StopFire()
    {
        firing = false;
    }

    std::vector<Bullet> &GetProjectiles() { return projectiles; }
    const sf::Texture &GetProjectileTexture() const { return projectileTexture; }
    int GetDamage() const { return damage; }

    void Load()
    {
        if (!projectileTexture.loadFromFile("../assets/bullet.png"))
        {
            std::cout << "Failed to load bullet texture\n";
            return;
        }
    }

    void Update(float deltaTime)
    {
        cooldownTimer -= deltaTime;

        if (firing && cooldownTimer <= 0.f)
        {
            cooldownTimer = 1 / fireRate;

            projectiles.push_back(currentProjectile);
        }

        for (Bullet &proj : projectiles)
        {
            proj.sprite.move({cos(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime, sin(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime});

            if ((proj.sprite.getPosition().x <= 0 || proj.sprite.getPosition().x >= 1600))
                proj.expired = true;

            if (proj.sprite.getPosition().y <= 0 || proj.sprite.getPosition().y >= 1600)
                proj.expired = true;
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](Bullet &proj)
                           {
                               return proj.IsExpired();
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const Bullet &projectile : projectiles)
            window.draw(projectile.GetSprite());
    }

private:
    sf::Texture projectileTexture{};
    Bullet currentProjectile{projectileTexture, sf::Vector2f{1.f, 1.f}, sf::Vector2f{1.f, 1.f}, 0, 1.f};
    std::vector<Bullet> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};