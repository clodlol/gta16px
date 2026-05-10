#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Collidable.h"
#include "utils.h"

#define TILE_SIZE_FLAME 400
#define PI 3.14159

class FlameProjectile : public Collidable
{

public:
    FlameProjectile(const sf::Texture &tex, const sf::Vector2f &ogn, const sf::Vector2f &dir, int dmg, float vel, float maxDist) : sprite(tex), direction(dir), origin(ogn), damage(dmg), velocity(vel), maxDistance(maxDist)
    {
        sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_FLAME, TILE_SIZE_FLAME}));
        sprite.setOrigin({TILE_SIZE_FLAME / 2, TILE_SIZE_FLAME / 2});
        sprite.scale({8.f / TILE_SIZE_FLAME, 8.f / TILE_SIZE_FLAME});
        sprite.rotate(direction.angle());
        sprite.setPosition(origin);
    }

    sf::FloatRect GetBounds() const override { return sprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return sprite; }

    const sf::Vector2f &GetOrigin() const { return origin; }
    const sf::Vector2f &GetDirection() const { return direction; }

    int GetDamage() const { return damage; }
    float GetVelocity() const { return velocity; }

    bool IsExpired() const { return ((origin - sprite.getPosition()).length()) > maxDistance; }

    friend class FlameGun;

private:
    sf::Sprite sprite;

    sf::Vector2f origin{1.f, 1.f};
    sf::Vector2f direction{1.f, 1.f};

    int damage = 10;
    float velocity = 500.f;
    float maxDistance = 100.f;
};

class FlameGun
{
public:
    FlameGun(float vel, float rate, int dmg, int inty, float spr) : velocity(vel), fireRate(rate), damage(dmg), intensity(inty), spread(spr)
    {
        cooldownTimer = 1.f / fireRate;
    }

    void Fire(const FlameProjectile &projectile)
    {
        currentProjectile = projectile;
        firing = true;
    }

    void StopFire()
    {
        firing = false;
    }

    const std::vector<FlameProjectile> &GetProjectiles() const { return projectiles; }
    const sf::Texture &GetProjectileTexture() const { return flameProjectileTexture; }

    int GetDamage() const { return damage; }

    void Load()
    {
        if (!flameProjectileTexture.loadFromFile("../assets/flame.gif"))
        {
            std::cout << "Error while loading flame gun projectile texture\n";
            return;
        }
    }

    void Update(float deltaTime)
    {
        cooldownTimer -= deltaTime;

        if (firing && cooldownTimer <= 0.f)
        {
            cooldownTimer = 1 / fireRate;

            for (int i = 0; i < intensity; ++i)
            {
                float randomAngle = currentProjectile.direction.angle().asRadians() + (randomNumGen(-PI / 10.f * spread, PI / 10.f * spread));
                currentProjectile.direction = {cos(randomAngle), sin(randomAngle)};
                currentProjectile.velocity = randomNumGen(0.1, currentProjectile.velocity);
                projectiles.push_back(currentProjectile);
            }
        }

        for (FlameProjectile &proj : projectiles)
        {
            proj.sprite.move({cos(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime, sin(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime});
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](FlameProjectile &proj)
                           {
                               return proj.IsExpired();
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const FlameProjectile &projectile : projectiles)
            window.draw(projectile.GetSprite());
    }

private:
    sf::Texture flameProjectileTexture{};
    FlameProjectile currentProjectile{flameProjectileTexture, sf::Vector2f{1.f, 1.f}, sf::Vector2f{1.f, 1.f}, 0, 1.f, 1.f};
    std::vector<FlameProjectile> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;
    int intensity = 5;   // how many flames per burst
    float spread = 0.5f; // how much spread, float from 0 to 1

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};

// template <typename T>
// class FlameGun
// {
//     static_assert(
//         std::is_base_of<AdvancedProjectile, T>::value,
//         "FlameGun.h: T must inherit from AdvancedProjectile");

// public:
//     FlameGun(float vel, float rate, int dmg)
//         : velocity(vel),
//           fireRate(rate),
//           damage(dmg)
//     {
//         cooldownTimer = 1.f / fireRate;

//         std::srand(
//             static_cast<unsigned>(std::time(nullptr)));
//     }

//     void Fire(const T &projectile)
//     {
//         currentProjectile = projectile;
//         firing = true;
//     }

//     void StopFire()
//     {
//         firing = false;
//     }

//     const std::vector<T> &GetProjectiles() const
//     {
//         return projectiles;
//     }

//     const sf::Texture &GetProjectileTexture() const
//     {
//         return projectileTexture;
//     }

//     int GetDamage() const
//     {
//         return damage;
//     }

//     float GetVelocity() const
//     {
//         return velocity;
//     }

//     void Load(std::filesystem::path filePath)
//     {
//         if (!projectileTexture.loadFromFile(filePath))
//         {
//             std::cout
//                 << "FlameGun.Load: Failed to load texture located at "
//                 << filePath
//                 << "\n";

//             return;
//         }
//     }

//     void Update(float deltaTime)
//     {
//         cooldownTimer -= deltaTime;

//         if (firing && cooldownTimer <= 0.f) // Spawning of flames {
//             cooldownTimer = 1.f / fireRate;

//         // Multiple flames per burst

//         for (int i = 0; i < 5; i++)
//         {
//             T sprayedFlame = currentProjectile;

//             float baseAngle =
//                 sprayedFlame
//                     .GetDirection()
//                     .angle()
//                     .asRadians();

//             // -15 to +15 degree spread     //if spraying doesnt look visually appealing then we'll reduce the angle
//             float randomOffset =
//                 ((std::rand() % 31) - 15) * 3.14159265f / 180.f;

//             float finalAngle =
//                 baseAngle + randomOffset;

//             sf::Vector2f sprayedDirection(
//                 std::cos(finalAngle),
//                 std::sin(finalAngle));

//             sprayedFlame.direction =
//                 sprayedDirection;

//             sprayedFlame.sprite.setRotation(
//                 sf::radians(finalAngle));

//             // Small random speed variance added so they dont look the same everytime
//             sprayedFlame.velocity +=
//                 ((std::rand() % 20) - 10);

//             projectiles.push_back(sprayedFlame);
//         }
//     }

//     // updation and deletion of flames

//     projectiles.erase(

//         std::remove_if(

//             projectiles.begin(),
//             projectiles.end(),

//             [&](T &flame)
//             {
//                 float moveDistance =
//                     (velocity + flame.GetVelocity()) * deltaTime;

//                 sf::Vector2f movement(

//                     std::cos(
//                         flame
//                             .GetDirection()
//                             .angle()
//                             .asRadians()) *
//                         moveDistance,

//                     std::sin(
//                         flame
//                             .GetDirection()
//                             .angle()
//                             .asRadians()) *
//                         moveDistance);

//                 flame.sprite.move(movement);

//                 flame.AddDistance(moveDistance);

//                 // Remove expired flames
//                 if (flame.IsExpired())
//                     return true;

//                 // Optional bounds cleanup
//                 if (flame.GetSprite().getPosition().x <= 0 ||
//                     flame.GetSprite().getPosition().x >= 1600)
//                 {
//                     return true;
//                 }

//                 if (flame.GetSprite().getPosition().y <= 0 ||
//                     flame.GetSprite().getPosition().y >= 1600)
//                 {
//                     return true;
//                 }

//                 return false;
//             }),

//         projectiles.end());
// }

// void
// Draw(sf::RenderWindow &window)
// {
//     for (const T &projectile : projectiles)
//     {
//         window.draw(projectile.GetSprite());
//     }
// }

// private:
// sf::Texture projectileTexture{};

// T currentProjectile{
//     projectileTexture,
//     sf::Vector2f{1.f, 1.f},
//     sf::Vector2f{1.f, 1.f},
//     0,
//     1.f,
//     100.f};

// std::vector<T> projectiles;

// float velocity = 1.f;
// float fireRate = 1.f;

// int damage = 0;

// bool firing = false;

// float cooldownTimer = 1.f;
// }
// ;
