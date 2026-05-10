#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <type_traits>

#include "InputManager.h"
#include "Collidable.h"

#define TILE_SIZE_BULLET 225

#include <SFML/Graphics.hpp>

template <typename T>
class Gun
{
    static_assert(std::is_base_of<Collidable, T>::value, "Gun.h: T must inherit from BaseBullet");

public:
    Gun(float vel, float rate, int dmg) : velocity(vel), fireRate(rate), damage(dmg)
    {
        cooldownTimer = 1.f / fireRate;
    }

    void Fire(const T &projectile)
    {
        currentProjectile = projectile;
        firing = true;
    }

    void StopFire()
    {
        firing = false;
    }

    const std::vector<T> &GetProjectiles() const { return projectiles; }
    const sf::Texture &GetProjectileTexture() const { return projectileTexture; }
    int GetDamage() const { return damage; }

    void Load(std::filesystem::path filePath)
    {
        if (!projectileTexture.loadFromFile(filePath))
        {
            std::cout << "Gun.Load: Failed to load texture located at " << filePath << "\n";
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

        for (T &proj : projectiles)
        {
            proj.sprite.move({cos(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime, sin(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime});
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](T &proj)
                           {
                               return proj.IsExpired();
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const T &projectile : projectiles)
            window.draw(projectile.GetSprite());
    }

private:
    sf::Texture projectileTexture{};
    T currentProjectile{projectileTexture, sf::Vector2f{1.f, 1.f}, sf::Vector2f{1.f, 1.f}, 0, 1.f};
    std::vector<T> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};

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

    bool IsExpired() const
    {
        if ((sprite.getPosition().x <= 0 || sprite.getPosition().x >= 1600))
            return true;

        if (sprite.getPosition().y <= 0 || sprite.getPosition().y >= 1600)
            return true;

        return false;
    }

    friend class Gun<Bullet>;

private:
    sf::Sprite sprite;

    sf::Vector2f origin{1.f, 1.f};
    sf::Vector2f direction{1.f, 1.f};

    int damage;
    float velocity;
};