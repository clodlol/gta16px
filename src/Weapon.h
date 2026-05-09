#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <type_traits>

#include "InputManager.h"
#include "Projectile.h"

#define TILE_SIZE_BULLET 225

template <typename T>
class Weapon
{
    static_assert(std::is_base_of<Projectile, T>::value, "T must inherit from Projectile");

public:
    Weapon(float vel, float rate, int dmg) : velocity{vel}, fireRate{rate}, damage{dmg}
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

    void Load(std::filesystem::path filePath)
    {
        if (!projectileTexture.loadFromFile(filePath))
        {
            std::cout << "Failed to load texture located at " << filePath << "\n";
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

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [&](T &proj)
                           {
                               if (proj.GetSprite().getPosition().x <= 0 || proj.GetSprite().getPosition().x >= 1600)
                                   return true;

                               if (proj.GetSprite().getPosition().y <= 0 || proj.GetSprite().getPosition().y >= 1600)
                                   return true;

                               proj.MoveSprite({cos(proj.GetDirection().asRadians()) * velocity * deltaTime, sin(proj.GetDirection().asRadians()) * velocity * deltaTime});

                               return false;
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const T &projectile : projectiles)
            window.draw(projectile.GetSprite());
    }

private:
    sf::Texture projectileTexture;
    T currentProjectile{projectileTexture, sf::Vector2f{0, 0}, sf::Angle{}, damage};
    std::vector<T> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};

class Bullet : public Projectile
{
public:
    Bullet(const sf::Texture &tex, const sf::Vector2f &ogn, const sf::Angle &dir, int dmg) : sprite(tex), origin(ogn), direction(dir), damage(dmg)
    {
        sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_BULLET, TILE_SIZE_BULLET}));
        sprite.setOrigin({TILE_SIZE_BULLET / 2, TILE_SIZE_BULLET / 2});
        sprite.scale({8.f / TILE_SIZE_BULLET, 8.f / TILE_SIZE_BULLET});
        sprite.rotate(direction);
        sprite.setPosition(origin);
    }

    sf::FloatRect GetBounds() const override { return sprite.getGlobalBounds(); }

    const sf::Sprite &GetSprite() const override { return sprite; }

    const sf::Vector2f &GetOrigin() const override { return origin; }
    const sf::Angle &GetDirection() const override { return direction; }
    int GetDamage() const override { return damage; }

    void MoveSprite(const sf::Vector2f &difference)
    {
        sprite.move(difference);
    }

    friend class Weapon<Bullet>;

private:
    sf::Sprite sprite;

    sf::Vector2f origin;
    sf::Angle direction;

    int damage;
};